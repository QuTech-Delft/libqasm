/**
 * \dir
 * Contains the \ref func-gen "generator" for the default cQASM-exposed
 * functions for use in expressions.
 *
 * \file
 * Source file for \ref func-gen.
 *
 * \page func-gen func-gen
 * func-gen is a simple code generator (always the same output) for the various
 * functions usable within expressions within cQASM.
 *
 * A generator is used because there is a very high degree of repetition in this
 * code. For example, as you might imagine, the code for the "sin()" function
 * is the same as the code for "cos()", aside from a different cmath function
 * being called when constant propagation is performed. There are already 12 of
 * these just for trigonometry on reals, not to mention duplication for complex
 * numbers, and so on.
 *
 * Adding a simple function with a similar prototype to what already exists
 * should be fairly easy through copypasting existing code, even within the
 * generator. The generator code isn't very complicated.
 */

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * Namespace for the \ref func-gen program.
 */
namespace func_gen {

/**
 * Represents a previously generated function, to be added in the register
 * function once it is generated.
 */
class Function {
public:

    /**
     * Name of the function implementation as generated in the C++ file.
     */
    const std::string cpp_name;

    /**
     * Name of the function as exposed to cQASM.
     */
    const std::string cqasm_name;

    /**
     * Argument type spec for overload resolution, type checking, and type
     * promotion. See cqasm::types::from_spec.
     */
    const std::string cqasm_args;

private:

    /**
     * Generates the C++ name for the function.
     */
    static std::string unique_name(const std::string &name, const std::string &args) {
        if (name == "operator+") {
            return "op_add_" + args;
        } else if (name == "operator-") {
            if (args.size() == 1) {
                return "op_neg_" + args;
            } else {
                return "op_sub_" + args;
            }
        } else if (name == "operator*") {
            return "op_mul_" + args;
        } else if (name == "operator/") {
            return "op_div_" + args;
        } else if (name == "operator//") {
            return "op_int_div_" + args;
        } else if (name == "operator%") {
            return "op_mod_" + args;
        } else if (name == "operator**") {
            return "op_pow_" + args;
        } else if (name == "operator<<") {
            return "op_shl_" + args;
        } else if (name == "operator>>") {
            return "op_sra_" + args;
        } else if (name == "operator>>>") {
            return "op_srl_" + args;
        } else if (name == "operator==") {
            return "op_eq_" + args;
        } else if (name == "operator!=") {
            return "op_ne_" + args;
        } else if (name == "operator>=") {
            return "op_ge_" + args;
        } else if (name == "operator>") {
            return "op_gt_" + args;
        } else if (name == "operator<=") {
            return "op_le_" + args;
        } else if (name == "operator<") {
            return "op_lt_" + args;
        } else if (name == "operator~") {
            return "op_binv_" + args;
        } else if (name == "operator&") {
            return "op_band_" + args;
        } else if (name == "operator^") {
            return "op_bxor_" + args;
        } else if (name == "operator|") {
            return "op_bor_" + args;
        } else if (name == "operator!") {
            return "op_linv_" + args;
        } else if (name == "operator&&") {
            return "op_land_" + args;
        } else if (name == "operator^^") {
            return "op_lxor_" + args;
        } else if (name == "operator||") {
            return "op_lor_" + args;
        } else if (name == "operator?:") {
            return "op_tcnd_" + args;
        } else {
            return "fn_" + name + "_" + args;
        }
    }

public:

    /**
     * Constructs a normal function. name must be a valid identifier.
     */
    Function(const std::string &name, const std::string &args)
        : cpp_name(unique_name(name, args)), cqasm_name(name), cqasm_args(args)
    {}

};

/**
 * Generator class.
 */
class Generator {
private:

    /**
     * Vector of all functions generated so far.
     */
    std::vector<Function> funcs_;

    /**
     * Output stream for the header file.
     */
    std::ofstream header_ofs_;

    /**
     * Output stream for the source file.
     */
    std::ofstream source_ofs_;

    /**
     * Version name.
     */
    std::string version_;

    /**
     * Generates the function that registers the functions in a FunctionTable.
     */
    void generate_register_function() {
        header_ofs_ << R"(
/**
 * Registers a bunch of functions usable during constant propagation into the
 * given function table.
 */
void register_into(resolver::FunctionTable &table);
)";
        source_ofs_ << R"(
/**
 * Registers a bunch of functions usable during constant propagation into the
 * given function table.
 */
void register_into(resolver::FunctionTable &table) {
)";
        for (const auto &func : funcs_) {
            source_ofs_ << "    table.add(";
            source_ofs_ << "\"" << func.cqasm_name << "\", ";
            source_ofs_ << "types::from_spec(\"" << func.cqasm_args << "\"), ";
            source_ofs_ << func.cpp_name;
            source_ofs_ << ");" << std::endl;
        }
        source_ofs_ << std::endl << "}" << std::endl;
    }

    /**
     * Generates the function header in both the header and source file. The
     * start of the function body must follow. This also registers the function
     * to be added to the function that populates FunctionTables.
     */
    void generate_impl_header(const Function &func) {
        auto proto = "values::Value " + func.cpp_name + "(const values::Values &v)";
        header_ofs_ << proto << ";" << std::endl;
        source_ofs_ << std::endl << proto << " {" << std::endl;
        funcs_.push_back(func);
    }

    /**
     * Like generate_impl_header(), but in addition, generates some boilerplate
     * code for safely casting all the arguments to constant values (throwing
     * a compile error if they're not constant) and storing them in variables
     * a..z. This is, of course, only applicable for functions that are only
     * evaluable during semantic analysis.
     */
    void generate_const_impl_header(const Function &func) {
        generate_impl_header(func);
        source_ofs_ << "    values::check_const(v);" << std::endl;
        size_t index = 0;
        for (auto arg_typ : func.cqasm_args) {
            source_ofs_ << "    auto " << (char)('a' + index) << " = v[" << index << "]";
            switch (arg_typ) {
                case 'b': source_ofs_ << "->as_const_bool()->value"; break;
                case 'a': source_ofs_ << "->as_const_axis()->value"; break;
                case 'i': source_ofs_ << "->as_const_int()->value"; break;
                case 'r': source_ofs_ << "->as_const_real()->value"; break;
                case 'c': source_ofs_ << "->as_const_complex()->value"; break;
                case 'm': source_ofs_ << "->as_const_real_matrix()->value"; break;
                case 'u':
                case 'n': source_ofs_ << "->as_const_complex_matrix()->value"; break;
                case 's': source_ofs_ << "->as_const_string()->value"; break;
                case 'j': source_ofs_ << "->as_const_json()->value"; break;
                default: throw std::invalid_argument("unknown arg type");
            }
            source_ofs_ << ";" << std::endl;
            index++;
        }
    }

    /**
     * Generates the end of a function body, with an appropriate return
     * statement. return_type must be one of:
     *
     *  - 'b': returns a ConstBool, return_expr must be a primitive::Bool.
     *  - 'a': returns a ConstAxis, return_expr must be a primitive::Axis.
     *  - 'i': returns a ConstInt, return_expr must be a primitive::Int.
     *  - 'r': returns a ConstReal, return_expr must be a primitive::Real.
     *  - 'c': returns a ConstComplex, return_expr must be a primitive::Complex.
     *  - 'm': returns a ConstRealMatrix, return_expr must be a primitive::RMatrix.
     *  - 'n': returns a ConstComplexMatrix, return_expr must be a primitive::CMatrix.
     *  - 's': returns a ConstString, return_expr must be a primitive::Str.
     *  - 'j': returns a ConstJson, return_expr must be a primitive::Str.
     *  - 'Q': returns a QubitRefs, return_expr must be a Many<ConstInt>.
     *  - 'B': returns a BitRefs, return_expr must be a Many<ConstInt>.
     */
    void generate_impl_footer(const std::string &return_expr, char return_type) {
        source_ofs_ << "    return tree::make<values::";
        switch (return_type) {
            case 'b': source_ofs_ << "ConstBool"; break;
            case 'a': source_ofs_ << "ConstAxis"; break;
            case 'i': source_ofs_ << "ConstInt"; break;
            case 'r': source_ofs_ << "ConstReal"; break;
            case 'c': source_ofs_ << "ConstComplex"; break;
            case 'm': source_ofs_ << "ConstRealMatrix"; break;
            case 'n': source_ofs_ << "ConstComplexMatrix"; break;
            case 's': source_ofs_ << "ConstString"; break;
            case 'j': source_ofs_ << "ConstJson"; break;
            case 'Q': source_ofs_ << "QubitRefs"; break;
            case 'B': source_ofs_ << "BitRefs"; break;
            default: throw std::invalid_argument("unknown type code");
        }
        source_ofs_ << ">(" << return_expr << ");" << std::endl;
        source_ofs_ << "}" << std::endl;
    }

public:

    /**
     * Generates a basic constant scalar function, such as integer addition for
     * instance. name must be the cQASM name for the function (either just the
     * function name or operator+ etc), return_type must be the return type
     * code, arg_types must be the argument type codes, and impl must be a
     * C++ expression implementing the function, operating on variables a..z
     * representing the arguments, and returning the primitive value associated
     * with return_type.
     */
    void generate_const_scalar_op(
        const std::string &name,
        const char return_type,
        const std::string &arg_types,
        const std::string &impl
    ) {
        generate_const_impl_header(Function(name, arg_types));
        generate_impl_footer(impl, return_type);
    }

    /**
     * Constructs a generator for the function table.
     */
    Generator(
        const std::string &header_filename,
        const std::string &source_filename,
        const std::string &version
    ) :
        funcs_(),
        header_ofs_(header_filename),
        source_ofs_(source_filename),
        version_(version)
    {
        // Check that the files were opened properly.
        if (!header_ofs_.is_open()) {
            throw std::runtime_error("failed to open header file");
        }
        if (!source_ofs_.is_open()) {
            throw std::runtime_error("failed to open source file");
        }

        // Print the headers for the header and source files.
        auto pos = header_filename.rfind('/');
        auto header_name = (pos == header_filename.npos) ? header_filename : header_filename.substr(pos + 1);
        header_ofs_ << R"(
/** \file
 * Header file generated by \ref func-gen.
 */

#pragma once

#include ")" << version_ << R"(/cqasm-resolver.hpp"

namespace cqasm {
namespace )" << version_ << R"( {
namespace functions {
)";
        source_ofs_ << R"(
/** \file
 * Source file generated by \ref func-gen.
 */

#include <cmath>
#include <complex>
#include ")" << version_ << R"(/)" << header_name << R"("

namespace cqasm {
namespace )" << version_ << R"( {

/**
 * Namespace for the functions generated by \ref func-gen.
 */
namespace functions {

/**
 * Division that always rounds down (towards negative infinity), like Python's
 * integer division. Because rounding to zero is a useless mechanic.
 */
static int64_t div_floor(int64_t a, int64_t b) {
    int64_t res = a / b;
    int64_t rem = a % b;
    // Correct division result downwards if up-rounding happened,
    // (for non-zero remainder of sign different than the divisor).
    int64_t corr = (rem != 0 && ((rem < 0) != (b < 0)));
    return res - corr;
}

/**
 * Modulo to go along with div_floor.
 */
static int64_t mod_floor(int64_t a, int64_t b) {
    int64_t rem = a % b;
    if (rem != 0 && ((rem < 0) != (b < 0))) {
        rem += b;
    }
    return rem;
}

)";
    }

    /**
     * Finishes writing the header & source file, then destroys the generator.
     */
    ~Generator() {
        generate_register_function();
        auto footer = R"(
} // namespace functions
} // namespace )" + version_ + R"(
} // namespace cqasm
)";
        source_ofs_ << footer;
        header_ofs_ << footer;
    }

};

} // namespace func_gen

/**
 * Entry point.
 */
int main(int argc, char *argv[]) {

    // Check command line.
    if (argc != 4) {
        std::cerr << "Usage: func-gen <header.hpp> <source.cpp> <version>" << std::endl;
        return 1;
    }

    // Construct the generator.
    func_gen::Generator generator{argv[1], argv[2], argv[3]};

    // Basic scalar arithmetic operators.
    generator.generate_const_scalar_op("operator+", 'c', "cc", "a + b");
    generator.generate_const_scalar_op("operator+", 'r', "rr", "a + b");
    generator.generate_const_scalar_op("operator+", 'i', "ii", "a + b");
    generator.generate_const_scalar_op("operator+", 's', "ss", "a + b");
    generator.generate_const_scalar_op("operator-", 'c', "cc", "a - b");
    generator.generate_const_scalar_op("operator-", 'r', "rr", "a - b");
    generator.generate_const_scalar_op("operator-", 'i', "ii", "a - b");
    generator.generate_const_scalar_op("operator-", 'c', "c", "-a");
    generator.generate_const_scalar_op("operator-", 'r', "r", "-a");
    generator.generate_const_scalar_op("operator-", 'i', "i", "-a");
    generator.generate_const_scalar_op("operator*", 'c', "cc", "a * b");
    generator.generate_const_scalar_op("operator*", 'r', "rr", "a * b");
    generator.generate_const_scalar_op("operator*", 'i', "ii", "a * b");
    generator.generate_const_scalar_op("operator/", 'c', "cc", "a / b");
    generator.generate_const_scalar_op("operator/", 'r', "rr", "a / b");
    generator.generate_const_scalar_op("operator//", 'i', "ii", "div_floor(a, b)");
    generator.generate_const_scalar_op("operator%", 'i', "ii", "mod_floor(a, b)");
    generator.generate_const_scalar_op("operator**", 'c', "cc", "std::pow(a, b)");
    generator.generate_const_scalar_op("operator**", 'r', "rr", "std::pow(a, b)");

    // Relational operators.
    generator.generate_const_scalar_op("operator==", 'b', "cc", "a == b");
    generator.generate_const_scalar_op("operator!=", 'b', "cc", "a != b");
    for (const char *type : {"rr", "ii", "bb"}) {
        generator.generate_const_scalar_op("operator==", 'b', type, "a == b");
        generator.generate_const_scalar_op("operator!=", 'b', type, "a != b");
        generator.generate_const_scalar_op("operator>=", 'b', type, "a >= b");
        generator.generate_const_scalar_op("operator>", 'b', type, "a > b");
        generator.generate_const_scalar_op("operator<=", 'b', type, "a <= b");
        generator.generate_const_scalar_op("operator<", 'b', type, "a < b");
    }

    // Bitwise operators.
    generator.generate_const_scalar_op("operator~", 'i', "i", "~a");
    generator.generate_const_scalar_op("operator&", 'i', "ii", "a & b");
    generator.generate_const_scalar_op("operator^", 'i', "ii", "a ^ b");
    generator.generate_const_scalar_op("operator|", 'i', "ii", "a | b");
    generator.generate_const_scalar_op("operator<<", 'i', "ii", "a << b");
    generator.generate_const_scalar_op("operator>>", 'i', "ii", "a >> b");
    generator.generate_const_scalar_op("operator>>>", 'i', "ii", "(int64_t)(((uint64_t)a) >> b)");

    // Logical operators.
    generator.generate_const_scalar_op("operator!", 'b', "b", "!a");
    generator.generate_const_scalar_op("operator&&", 'b', "bb", "a && b");
    generator.generate_const_scalar_op("operator^^", 'b', "bb", "!a ^ !b");
    generator.generate_const_scalar_op("operator||", 'b', "bb", "a || b");

    // Ternary conditional.
    for (const char *type : {"bcc", "brr", "bii", "bbb"}) {
        generator.generate_const_scalar_op("operator?:", type[1], type, "a ? b : c");
    }

    // Scalar root, exponent, and trigonometric functions for reals and complex
    // numbers.
    for (const char *type : {"c", "r"}) {
        generator.generate_const_scalar_op("sqrt", type[0], type, "std::sqrt(a)");
        generator.generate_const_scalar_op("exp", type[0], type, "std::exp(a)");
        generator.generate_const_scalar_op("log", type[0], type, "std::log(a)");
        for (const char *prefix : {"", "a"}) {
            for (const char *suffix : {"", "h"}) {
                for (const char *func : {"sin", "cos", "tan"}) {
                    std::ostringstream ss;
                    ss << prefix << func << suffix;
                    generator.generate_const_scalar_op(
                        ss.str(), type[0], type, "std::" + ss.str() + "(a)");
                }
            }
        }
    }

    // Absolute value function.
    generator.generate_const_scalar_op("abs", 'r', "r", "std::abs(a)");
    generator.generate_const_scalar_op("abs", 'i', "i", "std::abs(a)");

    // Complex number manipulation functions.
    generator.generate_const_scalar_op("complex", 'c', "rr", "primitives::Complex(a, b)");
    generator.generate_const_scalar_op("polar", 'c', "rr", "std::polar<double>(a, b)");
    generator.generate_const_scalar_op("real", 'r', "c", "std::real<double>(a)");
    generator.generate_const_scalar_op("imag", 'r', "c", "std::imag<double>(a)");
    generator.generate_const_scalar_op("arg", 'r', "c", "std::arg<double>(a)");
    generator.generate_const_scalar_op("norm", 'r', "c", "std::norm<double>(a)");
    generator.generate_const_scalar_op("conj", 'c', "c", "std::conj<double>(a)");

    return 0;
}

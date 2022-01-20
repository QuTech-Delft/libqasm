/** \file
 * Contains the guts of the compatibility layer, parsing using
 * \ref cqasm::analyzer::Analyzer "Analyzer" and converting the result to an
 * \ref compiler::QasmRepresentation "old API AST".
 */

#pragma once
#ifndef QASM_NEW_TO_OLD_HPP

#include "cqasm.hpp"
#include "qasm_ast.hpp"

namespace compiler {

/**
 * Namespace containing the guts of the compatibility layer emulating the old
 * API while using the guts of the new one.
 */
namespace new_to_old {

/**
 * All instruction types supported by the old API based on the
 * types of their parameters alone.
 */
enum class ParameterType {
    NoArg,
    SingleBit,
    SingleQubit,
    SingleQubitInt,
    SingleQubitReal,
    SingleQubitMatrix,
    TwoQubit,
    TwoQubitReal,
    TwoQubitInt,
    ThreeQubit,
    MeasureParity,
    SingleInt,
    SingleString,
    // Confusingly and annoyingly, the not gate uses the
    // control bits attribute instead of the bits_ attribute
    // (like display does, for instance). So we have to
    // special-case it for compatibility.
    NotGate
};

/**
 * Converts a list of indices from the new API format to the old API
 * format.
 */
static NumericalIdentifiers convert_indices(const cqasm::tree::Many<cqasm::values::ConstInt> &indices) {
    NumericalIdentifiers retval;
    for (auto control_bit : indices) {
        retval.addToVector(control_bit->value);
    }
    return retval;
}

/**
 * Converts an axis from the new API format to the old API format.
 */
static std::string convert_axis(const cqasm::values::Value &value) {
    switch (value->as_const_axis()->value) {
        case cqasm::primitives::Axis::X: return "x";
        case cqasm::primitives::Axis::Y: return "y";
        case cqasm::primitives::Axis::Z: return "z";
    }
    throw std::runtime_error("unreachable code, corrupted axis enum");
}

/**
 * Converts an instruction from the new API format to the old API
 * format. May return null to optimize the instruction away.
 */
static Operation *convert_instruction(const cqasm::semantic::Instruction &instruction) {

    // Handle the normal arguments.
    Operation *op = nullptr;
    switch (instruction.instruction->get_annotation<ParameterType>()) {
        case ParameterType::NoArg:
            op = new Operation(
                instruction.instruction->name
            );
            break;
        case ParameterType::SingleBit:
            op = new Operation(
                instruction.instruction->name,
                Bits(convert_indices(instruction.operands[0]->as_bit_refs()->index))
            );
            break;
        case ParameterType::SingleQubit:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index))
            );
            break;
        case ParameterType::SingleQubitInt:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index)),
                (const int)instruction.operands[1]->as_const_int()->value
            );
            break;
        case ParameterType::SingleQubitReal:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index)),
                (const double)instruction.operands[1]->as_const_real()->value
            );
            break;
        case ParameterType::SingleQubitMatrix:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index))
            );
            {
                auto mat = instruction.operands[1]->as_const_complex_matrix()->value;
                std::vector<double> mat_elements;
                for (size_t row = 1; row <= mat.size_rows(); row++) {
                    for (size_t col = 1; col <= mat.size_cols(); col++) {
                        auto val = mat.at(row, col);
                        mat_elements.push_back(val.real());
                        mat_elements.push_back(val.imag());
                    }
                }
                op->setUMatrixElements(mat_elements);
            }
            break;
        case ParameterType::TwoQubit:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index)),
                Qubits(convert_indices(instruction.operands[1]->as_qubit_refs()->index))
            );
            break;
        case ParameterType::TwoQubitReal:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index)),
                Qubits(convert_indices(instruction.operands[1]->as_qubit_refs()->index)),
                instruction.operands[2]->as_const_real()->value
            );
            break;
        case ParameterType::TwoQubitInt:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index)),
                Qubits(convert_indices(instruction.operands[1]->as_qubit_refs()->index)),
                instruction.operands[2]->as_const_int()->value
            );
            break;
        case ParameterType::ThreeQubit:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index)),
                Qubits(convert_indices(instruction.operands[1]->as_qubit_refs()->index)),
                Qubits(convert_indices(instruction.operands[2]->as_qubit_refs()->index))
            );
            break;
        case ParameterType::MeasureParity:
            op = new Operation(
                instruction.instruction->name,
                Qubits(convert_indices(instruction.operands[0]->as_qubit_refs()->index)),
                convert_axis(instruction.operands[1]),
                Qubits(convert_indices(instruction.operands[2]->as_qubit_refs()->index)),
                convert_axis(instruction.operands[3])
            );
            break;
        case ParameterType::SingleInt:
            op = new Operation(
                instruction.instruction->name,
                instruction.operands[0]->as_const_int()->value
            );
            break;
        case ParameterType::SingleString:
            op = new Operation(
                instruction.instruction->name,
                instruction.operands[0]->as_const_string()->value
            );
            break;
        case ParameterType::NotGate:
            op = new Operation(
                instruction.instruction->name
            );
            op->setControlBits(Bits(convert_indices(instruction.operands[0]->as_bit_refs()->index)));
            break;
    }
    if (!op) {
        throw cqasm::error::AnalysisError(
            "unsupported parameter signature for old API",
            &instruction
        );
    }

    // Handle the condition code.
    if (auto control_bits = instruction.condition->as_bit_refs()) {
        op->setControlBits(Bits(convert_indices(control_bits->index)));
    } else if (auto control_const = instruction.condition->as_const_bool()) {
        if (!control_const->value) {
            // Constant false condition. Is at the time
            // of writing already optimized away
            // earlier, but it can't hurt to
            // double-check.
            return nullptr;
        }
    } else {
        throw cqasm::error::AnalysisError(
            "unsupported condition for old API",
            &*instruction.condition
        );
    }

    return op;
}

/**
 * Checks the parse results for errors, runs semantic analysis on
 * the result, checks *that* for errors, and then converts from the
 * new API format to the old one.
 */
static void handle_parse_result(QasmRepresentation &qasm, cqasm::parser::ParseResult &&result) {

    // Throw an error if parsing failed.
    if (!result.errors.empty()) {
        throw std::runtime_error(result.errors[0]);
    }

    // Create the semantic analyzer.
    auto analyzer = cqasm::analyzer::Analyzer{"1.0"};
    analyzer.register_default_functions_and_mappings();

    // The old library accepted the depolarizing_channel error model
    // with *any* number of float arguments. I'm just randomly
    // assuming 50 arguments is enough... Just increase this if more
    // are needed.
    std::ostringstream args;
    for (int i = 0; i <= 50; i++) {
        analyzer.register_error_model("depolarizing_channel", args.str());
        args << "r";
    }

    // Register the instructions that were baked into the old
    // grammar.
#define REG(typ, ...) analyzer.register_instruction_with_annotation<ParameterType>(ParameterType::typ, __VA_ARGS__)
    REG(NoArg, "measure_all", "", false, false);
    REG(MeasureParity, "measure_parity", "QaQa", false, false, false, true);
    REG(SingleQubit, "x", "Q");
    REG(SingleQubit, "y", "Q");
    REG(SingleQubit, "z", "Q");
    REG(SingleQubit, "i", "Q");
    REG(SingleQubit, "h", "Q");
    REG(SingleQubit, "x90", "Q");
    REG(SingleQubit, "y90", "Q");
    REG(SingleQubit, "mx90", "Q");
    REG(SingleQubit, "my90", "Q");
    REG(SingleQubit, "s", "Q");
    REG(SingleQubit, "sdag", "Q");
    REG(SingleQubit, "t", "Q");
    REG(SingleQubit, "tdag", "Q");
    REG(SingleQubitMatrix, "u", "Qu");
    REG(SingleQubit, "prep", "Q", false);
    REG(SingleQubit, "prep_x", "Q", false);
    REG(SingleQubit, "prep_y", "Q", false);
    REG(SingleQubit, "prep_z", "Q", false);
    REG(SingleQubit, "measure", "Q", false);
    REG(SingleQubit, "measure_x", "Q", false);
    REG(SingleQubit, "measure_y", "Q", false);
    REG(SingleQubit, "measure_z", "Q", false);
    REG(SingleQubitReal, "rx", "Qr");
    REG(SingleQubitReal, "ry", "Qr");
    REG(SingleQubitReal, "rz", "Qr");
    REG(TwoQubit, "cnot", "QQ");
    REG(TwoQubit, "cz", "QQ");
    REG(TwoQubit, "swap", "QQ");
    REG(TwoQubitReal, "cr", "QQr");
    REG(TwoQubitInt, "crk", "QQi");
    REG(ThreeQubit, "toffoli", "QQQ");
    REG(NotGate, "not", "B");
    REG(NoArg, "display", "", false, false);
    REG(SingleBit, "display", "B", false, false);
    REG(NoArg, "display_binary", "", false, false);
    REG(SingleBit, "display_binary", "B", false, false);
    REG(SingleInt, "skip", "i", false, false);
    REG(SingleQubitInt, "wait", "Qi", false, false);
    REG(SingleQubit, "barrier", "Q", false, false);
    REG(NoArg, "reset-averaging", "", false, false);
    REG(SingleQubit, "reset-averaging", "Q", false, false);
    REG(SingleString, "load_state", "s", false, false);
#undef REG

    // Run analysis.
    auto analysis_result = analyzer.analyze(*result.root->as_program());
    if (!analysis_result.errors.empty()) {
        throw std::runtime_error(analysis_result.errors[0]);
    }

    // Convert the new parse tree to the old AST classes.
    // GENERAL NOTE: the new operators all over the place introduce
    //   memory leaks. This is legacy behavior intentionally kept
    //   for backward compatibility.

    // Handle storage locations.
    if (analysis_result.root->num_qubits == 0) {
        throw std::runtime_error("qubits statement is missing");
    }
    qasm.qubitRegister(analysis_result.root->num_qubits);
    if (!analysis_result.root->variables.empty()) {
        throw std::runtime_error("variables are not supported");
    }

    // Copy version number (poorly).
    double version = analysis_result.root->version->items[0];
    if (analysis_result.root->version->items.size() > 1) {
        int sub = analysis_result.root->version->items[1];
        version += 0.1 * ((sub > 9) ? 9 : sub);
    }
    qasm.versionNumber(version);

    // Copy error model.
    if (!analysis_result.root->error_model.empty()) {
        std::vector<double> args;
        for (auto arg : analysis_result.root->error_model->parameters) {
            args.push_back(arg->as_const_real()->value);
        }
        qasm.setErrorModel(analysis_result.root->error_model->name, args);
    }

    // Copy subcircuits.
    auto &scs = qasm.getSubCircuits();
    for (auto subcircuit : analysis_result.root->subcircuits) {

        // The old API adds a default subcircuit automatically in
        // the QasmRepresentation constructor (so it always exists),
        // whereas the new API only adds a default one when it would
        // not be empty. The default subcircuit in the new API is
        // represented with an (otherwise illegal) empty name. So,
        // if we only add subcircuits with a nonempty name, and
        // always add bundles to the last subcircuit, we end up
        // doing the right thing.
        if (!subcircuit->name.empty()) {
            int line_number = 0;
            if (auto loc = subcircuit->get_annotation_ptr<cqasm::parser::SourceLocation>()) {
                line_number = loc->first_line;
            }
            SubCircuit sc {
                subcircuit->name.c_str(),
                (int)scs.numberOfSubCircuits(),
                line_number
            };
            sc.numberIterations(subcircuit->iterations);
            scs.addSubCircuit(sc);
        }

        // Add bundles to the last subcircuit.
        for (auto bundle : subcircuit->bundles) {

            // Construct the OperationsCluster for this bundle.
            OperationsCluster *opclus = nullptr;
            for (auto instruction : bundle->items) {

                // Convert the instruction.
                Operation *op = convert_instruction(*instruction);
                if (!op) {
                    continue;
                }

                // Add the operation to an operation cluster. When
                // this is the first instruction, we first construct
                // the cluster. This makes it a "serial" cluster.
                // When we receive another instruction, it's added
                // with the addParallelOperation method, which marks
                // the cluster as parallel.
                if (!opclus) {
                    int line_number = 0;
                    if (auto loc = instruction->get_annotation_ptr<cqasm::parser::SourceLocation>()) {
                        line_number = loc->first_line;
                    }
                    opclus = new OperationsCluster(op, line_number);
                } else {
                    opclus->addParallelOperation(op);
                }
            }

            // Add the cluster to the last subcircuit if the bundle
            // was nonempty.
            if (opclus) {
                scs.lastSubCircuit().addOperationsCluster(opclus);
            }
        }
    }

    // Copy mappings for as far as this is supported by the old API.
    for (auto mapping : analysis_result.root->mappings) {
        if (auto qubit_refs = mapping->value->as_qubit_refs()) {
            qasm.addMappings(mapping->name, convert_indices(qubit_refs->index), true);
        } else if (auto bit_refs = mapping->value->as_bit_refs()) {
            qasm.addMappings(mapping->name, convert_indices(bit_refs->index), false);
        }
    }

}

} // namespace new_to_old
} // namespace compiler

#endif

#include "v3x/register-core-functions.hpp"

#include <cassert>


namespace cqasm::v3x::function {

/**
 * Registers a bunch of core functions.
 * That is, functions supported by the language, and that cannot be evaluated at compile time.
 * For functions supported by the language that can be evaluated at compile time, see ConstEvalCoreFunction.
 */
void register_core_functions(analyzer::Analyzer *analyzer) {
    assert(analyzer);

    analyzer->register_core_function("operator-", "f", 'f');
    analyzer->register_core_function("operator-", "i", 'i');

    analyzer->register_core_function("operator+", "ff", 'f');
    analyzer->register_core_function("operator+", "ii", 'i');
    analyzer->register_core_function("operator-", "ff", 'f');
    analyzer->register_core_function("operator-", "ii", 'i');

    analyzer->register_core_function("operator*", "ff", 'f');
    analyzer->register_core_function("operator*", "ii", 'i');
    analyzer->register_core_function("operator/", "ff", 'f');
    analyzer->register_core_function("operator/", "ii", 'i');
    analyzer->register_core_function("operator%", "ii", 'i');

    analyzer->register_core_function("operator**", "ff", 'f');

    analyzer->register_core_function("operator==", "ff", 'b');
    analyzer->register_core_function("operator!=", "ff", 'b');
    analyzer->register_core_function("operator>=", "ff", 'b');
    analyzer->register_core_function("operator>", "ff", 'b');
    analyzer->register_core_function("operator<=", "ff", 'b');
    analyzer->register_core_function("operator<", "ff", 'b');

    analyzer->register_core_function("operator==", "ii", 'b');
    analyzer->register_core_function("operator!=", "ii", 'b');
    analyzer->register_core_function("operator>=", "ii", 'b');
    analyzer->register_core_function("operator>", "ii", 'b');
    analyzer->register_core_function("operator<=", "ii", 'b');
    analyzer->register_core_function("operator<", "ii", 'b');

    analyzer->register_core_function("operator==", "bb", 'b');
    analyzer->register_core_function("operator!=", "bb", 'b');
    analyzer->register_core_function("operator>=", "bb", 'b');
    analyzer->register_core_function("operator>", "bb", 'b');
    analyzer->register_core_function("operator<=", "bb", 'b');
    analyzer->register_core_function("operator<", "bb", 'b');

    analyzer->register_core_function("operator~", "i", 'i');
    analyzer->register_core_function("operator&", "ii", 'i');
    analyzer->register_core_function("operator^", "ii", 'i');
    analyzer->register_core_function("operator|", "ii", 'i');

    analyzer->register_core_function("operator<<", "ii", 'i');
    analyzer->register_core_function("operator>>", "ii", 'i');

    analyzer->register_core_function("operator!", "b", 'b');
    analyzer->register_core_function("operator&&", "bb", 'b');
    analyzer->register_core_function("operator^^", "bb", 'b');
    analyzer->register_core_function("operator||", "bb", 'b');

    analyzer->register_core_function("operator?:", "bff", 'f');
    analyzer->register_core_function("operator?:", "bii", 'i');
    analyzer->register_core_function("operator?:", "bbb", 'b');

    analyzer->register_core_function("sqrt", "f", 'f');
    analyzer->register_core_function("exp", "f", 'f');
    analyzer->register_core_function("log", "f", 'f');
    analyzer->register_core_function("sin", "f", 'f');
    analyzer->register_core_function("cos", "f", 'f');
    analyzer->register_core_function("tan", "f", 'f');
    analyzer->register_core_function("sinh", "f", 'f');
    analyzer->register_core_function("cosh", "f", 'f');
    analyzer->register_core_function("tanh", "f", 'f');
    analyzer->register_core_function("asin", "f", 'f');
    analyzer->register_core_function("acos", "f", 'f');
    analyzer->register_core_function("atan", "f", 'f');
    analyzer->register_core_function("asinh", "f", 'f');
    analyzer->register_core_function("acosh", "f", 'f');
    analyzer->register_core_function("atanh", "f", 'f');
    analyzer->register_core_function("abs", "f", 'f');
    analyzer->register_core_function("abs", "i", 'i');
}

} // namespace cqasm::v3x::function

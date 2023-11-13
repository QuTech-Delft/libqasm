#include "v1x/cqasm.hpp"

#include <cassert>  // assert
#include <iostream>  // cout
#include <stdexcept>  // runtime_error
#include <sstream>  // ostringstream

namespace cq1x = cqasm::v1x;

void say_hello() {
#ifdef NDEBUG
    std::cout << "libqasm: Hello World Release!\n";
#else
    std::cout << "libqasm: Hello World Debug!\n";
#endif

    // ARCHITECTURES
#ifdef _M_X64
    std::cout << "  libqasm: _M_X64 defined\n";
#endif

#ifdef _M_IX86
    std::cout << "  libqasm: _M_IX86 defined\n";
#endif

#ifdef _M_ARM64
    std::cout << "  libqasm: _M_ARM64 defined\n";
#endif

#if __i386__
    std::cout << "  libqasm: __i386__ defined\n";
#endif

#if __x86_64__
    std::cout << "  libqasm: __x86_64__ defined\n";
#endif

#if __aarch64__
    std::cout << "  libqasm: __aarch64__ defined\n";
#endif

    // Libstdc++
#if defined _GLIBCXX_USE_CXX11_ABI
    std::cout << "  libqasm: _GLIBCXX_USE_CXX11_ABI "<< _GLIBCXX_USE_CXX11_ABI << "\n";
#endif

    // COMPILER VERSIONS
#if _MSC_VER
    std::cout << "  libqasm: _MSC_VER" << _MSC_VER<< "\n";
#endif

#if _MSVC_LANG
    std::cout << "  libqasm: _MSVC_LANG" << _MSVC_LANG<< "\n";
#endif

#if __cplusplus
    std::cout << "  libqasm: __cplusplus" << __cplusplus<< "\n";
#endif

#if __INTEL_COMPILER
    std::cout << "  libqasm: __INTEL_COMPILER" << __INTEL_COMPILER<< "\n";
#endif

#if __GNUC__
    std::cout << "  libqasm: __GNUC__" << __GNUC__<< "\n";
#endif

#if __GNUC_MINOR__
    std::cout << "  libqasm: __GNUC_MINOR__" << __GNUC_MINOR__<< "\n";
#endif

#if __clang_major__
    std::cout << "  libqasm: __clang_major__" << __clang_major__<< "\n";
#endif

#if __clang_minor__
    std::cout << "  libqasm: __clang_minor__" << __clang_minor__<< "\n";
#endif

#if __apple_build_version__
    std::cout << "  libqasm: __apple_build_version__" << __apple_build_version__<< "\n";
#endif

    // SUBSYSTEMS
#if __MSYS__
    std::cout << "  libqasm: __MSYS__" << __MSYS__<< "\n";
#endif

#if __MINGW32__
    std::cout << "  libqasm: __MINGW32__" << __MINGW32__<< "\n";
#endif

#if __MINGW64__
    std::cout << "  libqasm: __MINGW64__" << __MINGW64__<< "\n";
#endif

#if __CYGWIN__
    std::cout << "  libqasm: __CYGWIN__" << __CYGWIN__<< "\n";
#endif
}

// The code in this file is taken directly from test/v1x/tutorial.cpp
// Comments have been removed
// googletest EXPECTs have been substituted with asserts
// And references to 'res/v1x/grover.cq' to '../../res/grover.cq'
int main() {
    auto result = cq1x::analyze("../../res/grover.cq");

    cq1x::analyze_string("version 1.0; qubits 1; h q[0]; measure q[0]; display");

    assert(result->num_qubits == 9);

    assert(!result->error_model.empty());
    assert(result->error_model->name == "depolarizing_channel");
    assert(result->error_model->parameters.size() == 1);
    assert(result->error_model->parameters[0]->as_const_real()->value == 0.001);

    assert(result->subcircuits.size() == 3);
    assert(result->subcircuits[0]->name == "init");
    assert(result->subcircuits[1]->name == "grover");
    assert(result->subcircuits[1]->iterations == 3);

    auto first_subcircuit = result->subcircuits[0];
    assert(first_subcircuit->bundles.size() == 2);
    assert(first_subcircuit->bundles[0]->items.size() == 1);
    assert(first_subcircuit->bundles[1]->items.size() == 2);
    auto hadamard_instruction = first_subcircuit->bundles[1]->items[0];
    assert(hadamard_instruction->name == "h");
    auto qubit_refs = hadamard_instruction->operands[0]->as_qubit_refs();

    assert(qubit_refs->index.size() == 4);
    assert(qubit_refs->index[0]->value == 0);
    assert(hadamard_instruction->operands[0]->as_qubit_refs()->index.size() == 4);
    qubit_refs->index.add(cqasm::tree::make<cq1x::values::ConstInt>(6));
    assert(hadamard_instruction->operands[0]->as_qubit_refs()->index.size() == 5);
    auto clone = hadamard_instruction->operands[0]->clone();
    qubit_refs->index.add(cqasm::tree::make<cq1x::values::ConstInt>(7));
    assert(hadamard_instruction->operands[0]->as_qubit_refs()->index.size() == 6);
    assert(clone->as_qubit_refs()->index.size() == 5);
    auto clone2 = clone->clone();
    assert(clone2.equals(clone));
    assert(*clone2 != *clone);
    assert(clone2 != clone);
    assert(clone2.get_ptr() != clone.get_ptr());
    clone2->as_qubit_refs()->index.add(cqasm::tree::make<cq1x::values::ConstInt>(7));
    assert(clone2 != clone);
    std::ostringstream ss;
    ss << clone;
    assert(ss.str() == R"(QubitRefs( # ../../res/grover.cq:10:9..15
  index: [
    ConstInt( # ../../res/grover.cq:10:11..14
      value: 0
    )
    ConstInt( # ../../res/grover.cq:10:11..14
      value: 1
    )
    ConstInt( # ../../res/grover.cq:10:11..14
      value: 2
    )
    ConstInt( # ../../res/grover.cq:10:11..14
      value: 3
    )
    ConstInt(
      value: 6
    )
  ]
)
)");
    assert(clone->get_annotation<cq1x::parser::SourceLocation>().filename == "../../res/grover.cq");
    struct MyAnnotationType {
        int number;
    };
    assert(!clone->has_annotation<MyAnnotationType>());
    assert(!clone->get_annotation_ptr<MyAnnotationType>());
    try {
        clone->get_annotation<MyAnnotationType>().number;
    } catch (std::runtime_error& err) {

    }
    clone->set_annotation(MyAnnotationType {42});
    assert(clone->has_annotation<MyAnnotationType>());
    assert(clone->get_annotation_ptr<MyAnnotationType>()->number == 42);
    assert(clone->get_annotation<MyAnnotationType>().number == 42);
    assert(clone->get_annotation<cq1x::parser::SourceLocation>().filename == "../../res/grover.cq");

    say_hello();
}

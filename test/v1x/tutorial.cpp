#include "v1x/cqasm.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace cq1x = cqasm::v1x;


TEST(tutorial, tutorial) {

    // This is the simplest form of reading a file. What you end up with is a
    // reference to the root node of the semantic tree when parsing succeeds
    // (cqasm::tree::One<cq1x::semantic::Program> to be specific), or the
    // error messages printed to stderr and a cq1x::analysis::AnalysisError
    // exception (derived from std::runtime_error) on failure.
    auto result = cq1x::analyze("res/v1x/grover.cq");

    // There is also an overload for a FILE* directly (with an optional second
    // argument for the filename used in error messages to refer back to the
    // code), and there's also cq1x::analyze_string():
    cq1x::analyze_string("version 1.0; qubits 1; h q[0]; measure q[0]; display");

    // Note that a semicolon is now a one-to-one replacement for a newline
    // almost everywhere in a cQASM file, the only exceptions being inside a
    // string or JSON object.

    // The above functions are just wrappers around the lower-level API's for
    // convenience; there is much more to access and configure if you need it.
    // For example, you can get access to the raw abstract syntax tree by using
    // parser::parse_file() or parser::parse_string(), or configure the
    // instruction set or error models by constructing your own
    // analyzer::Analyzer object and calling its analyze()/analyze_string()
    // methods. The most relevant files here are src/cqasm.cpp,
    // src/cqasm-parse-helper.cpp, src/cqasm-analyzer.cpp, and their respective
    // header files in include/. Have a look through them to get familiar with
    // them if you need this level of control.

    // Once you have your semantic tree, the number of qubits is right in the
    // root,
    EXPECT_EQ(result->num_qubits, 9);

    // and so is the error model. It'll be empty if none is specified. Because
    // the available error models and their parameter types can be configured,
    // you have to cast the parameters to the type you're expecting before you
    // can access them, hence the as_const_real(). libqasm has already done the
    // type check for you in this case, so it's safe to do this cast.
    EXPECT_FALSE(result->error_model.empty());
    EXPECT_EQ(result->error_model->name, "depolarizing_channel");
    EXPECT_EQ(result->error_model->parameters.size(), 1);
    EXPECT_EQ(result->error_model->parameters[0]->as_const_real()->value, 0.001);

    // All instructions are placed inside subcircuit nodes. Unlike the old API,
    // the "default" subcircuit that you get when you specify an instruction
    // before the first subcircuit is omitted when it'd be empty, so [0] returns
    // the first real subcircuit for this file.
    EXPECT_EQ(result->subcircuits.size(), 3);
    EXPECT_EQ(result->subcircuits[0]->name, "init");
    EXPECT_EQ(result->subcircuits[1]->name, "grover");
    EXPECT_EQ(result->subcircuits[1]->iterations, 3);

    // Subcircuits are further broken up into bundles (previously named
    // operation clusters). The instructions in a bundle always start in the
    // same cycle, whereas different bundles always start in different cycles.
    // The wait and skip instructions can be used to pass time faster; a skip
    // instruction takes exactly the specified number of cycles to complete,
    // whereas wait first waits for all issued instructions to complete, and
    // *then* waits the specified number of cycles. Note that this means that
    // a wait instruction is meaningless without additional information about
    // how long each instruction takes; libqasm is not currently aware of this
    // context.
    auto first_subcircuit = result->subcircuits[0];
    EXPECT_EQ(first_subcircuit->bundles.size(), 2);
    EXPECT_EQ(first_subcircuit->bundles[0]->items.size(), 1); // x oracle
    EXPECT_EQ(first_subcircuit->bundles[1]->items.size(), 2); // { h q[0:3] | h oracle }

    // Like error models, the supported set of instructions is configurable, so
    // accessing operands requires casting to the type you're expecting, and
    // also like error models, libqasm usually does the requisite type checking
    // for you. Therefore, once we observe that the second instruction is a
    // Hadamard gate, we know for sure that it has one operand, and that that
    // operand is a qubit reference.
    auto hadamard_insn = first_subcircuit->bundles[1]->items[0];
    EXPECT_EQ(hadamard_insn->name, "h");
    auto qubit_refs = hadamard_insn->operands[0]->as_qubit_refs();

    // Qubit and bit references in cQASM can refer to one or more qubits at a
    // time. The Hadamard gate we're looking at is an example of this, referring
    // to qubits 0 to 3 inclusive. That means a Hadamard gate is to be issued
    // in parallel to all three of those qubits. For multi-qubit gates, the
    // number of qubits referenced for each operand must obviously be the same
    // for that reason, otherwise these semantics make no sense! libqasm checks
    // this to make sure this is the case.
    EXPECT_EQ(qubit_refs->index.size(), 4); // four qubits
    EXPECT_EQ(qubit_refs->index[0]->value, 0); // first qubit index

    // Note that everything's using -> instead of . for accessing members,
    // implying that everything's a pointer. That's not entirely true; they're
    // std::shared_ptr objects wrapped inside libqasm's own structure. This
    // structure, of the most part, behaves like a (smart) pointer, with some
    // notable differences:
    //
    //  - instead of comparing to nullptr or casting to boolean to do a null
    //    check, you need to use the empty() method. This is primarily to avoid
    //    you from accidentally forgetting a ->value for constant integer nodes
    //    like above; C++ would implicitly cast the boolean indicating whether
    //    the node is filled to an integer otherwise!
    //
    //  - instead of using the new operator, you have to construct them with
    //    cqasm::tree::make<node_type>(...), where node_type is the type of the
    //    node you want to construct, and ... is the set of things you want to
    //    initialize it (which may be empty for default/empty values). The big
    //    advantage over using the new operator here is that libqasm's structure
    //    will clean up after itself once it goes out of scope everywhere (it's
    //    reference counted), much like std::shared_ptr (cqasm::tree::make is
    //    the analogue to std::make_shared).
    //
    //  - unlike pointers or shared_ptr's, cQASM's nodes all have a clone()
    //    member, so you can make a copy if you want. Just using the assignment
    //    operator just copies the pointer! Observe:

    // Still four qubits:
    EXPECT_EQ(hadamard_insn->operands[0]->as_qubit_refs()->index.size(), 4);

    // Adding one to a different reference to the same thing:
    qubit_refs->index.add(cqasm::tree::make<cq1x::values::ConstInt>(6));

    // Now we have five in the original.
    EXPECT_EQ(hadamard_insn->operands[0]->as_qubit_refs()->index.size(), 5);

    // But if we make a clone:
    auto clone = hadamard_insn->operands[0]->clone();

    // And add another one:
    qubit_refs->index.add(cqasm::tree::make<cq1x::values::ConstInt>(7));

    // We have six in the original, but still have five in the clone.
    EXPECT_EQ(hadamard_insn->operands[0]->as_qubit_refs()->index.size(), 6);
    EXPECT_EQ(clone->as_qubit_refs()->index.size(), 5);

    // Note that the clone() method knows what type it is. You wouldn't be able
    // to copy (in this case) cq1x::values::Node using its constructor, because
    // it's an abstract class. This is exactly why clone() exists.

    // libqasm's tree structures are generated by the small tree-gen tool next
    // to the src directory using a mini-language (yes; we're using a language
    // parser to help construct the code for a language parser). While it might
    // seem like overkill, there are some big advantages:
    //
    //  - all nodes are guaranteed to be free of typos affecting only a single
    //    node, so if one node works properly, all of them should;
    //  - all nodes are guaranteed to have the same interface, so there are no
    //    surprises to the user;
    //  - it's much less tedious to add and maintain things like equality
    //    operators and string dumps, that C++ unfortunately can't generate for
    //    you;
    //  - the *.tree files are easier to read because they're much more
    //    condensed than the resulting C++ code, to the point where they're
    //    almost self-documenting. Nevertheless, the generated code is no less
    //    readable than what a human would write for the same thing, simply
    //    because it's so repetitive.
    //
    // Four trees are currently defined, each in their own namespace:
    //
    //  - the abstract syntax tree (defined in src/cqasm-ast.tree, available
    //    through the cq1x::ast namespace);
    //
    //  - the semantic tree (defined in src/cqasm-semantic.tree, available
    //    through the cq1x::semantic namespace);
    //
    //  - the type tree, used to representing the types of values libqasm's type
    //    system knows about (defined in src/cqasm-types.tree, available through
    //    the cq1x::types namespace);
    //
    //  - the value tree, representing the actual values or references to values
    //    only known at runtime for libqasm's type system (defined in
    //    src/cqasm-types.tree, available through the cq1x::types namespace).
    //
    // The latter two "trees" just represent a single node at a time, so you
    // could argue they're not really trees; nevertheless, it made sense to use
    // the existing tree system for them as well, to make inheritance easier.
    //
    // Tree nodes are connected using One, Maybe, Many, and Any objects in the
    // parent node:
    //
    //  - One: exactly one child node is needed for the tree to be complete;
    //  - Maybe: like One, but the child node is optional;
    //  - Many: one or more child nodes are needed for the tree to be complete;
    //  - Any: like Many, but zero child nodes is also acceptable.
    //
    // One/Maybe behave like shared_ptr's, and Any/Many behave like vectors of
    // One nodes. We've already used all of them implicitly in the code above.
    // Note that there is no difference between One and Maybe, other than that
    // One signals that it's an error for it to be empty; a One object *can*
    // be empty in exceptional cases or while the tree is under construction.
    // The same thing goes for Any and Many. You can check for completeness of
    // an entire tree using is_complete() on the root node.
    //
    // All tree nodes and the One/Maybe/Many/Any nodes define an equality
    // operator. Note that this implements pointer-based equality: the
    // element(s) in the containers must point to the exact same objects for
    // two containers to be equal. However, a value-based equality operator is
    // also provided, using the function equals(). Note that this differs from
    // just dereferencing both sides and then testing equality, as this would
    // only do value-based equality for the root node and pointer-based equality
    // beyond that!
    auto clone2 = clone->clone();
    EXPECT_TRUE(clone2.equals(clone)); // compares by value
    EXPECT_TRUE(*clone2 != *clone); // compares only the root by value
    EXPECT_TRUE(clone2 != clone); // compares by pointer
    EXPECT_TRUE(clone2.get_ptr() != clone.get_ptr()); // also compares by pointer

    clone2->as_qubit_refs()->index.add(cqasm::tree::make<cq1x::values::ConstInt>(7));
    EXPECT_TRUE(clone2 != clone);

    // All tree nodes and the One/Maybe/Many/Any nodes also have operator<<
    // overloads for streams, dumping a pretty-printed representation of the
    // tree to the stream. For example:
    std::ostringstream ss;
    ss << clone;
    EXPECT_EQ(ss.str(),
R"(QubitRefs( # res/v1x/grover.cq:10:9..15
  index: [
    ConstInt( # res/v1x/grover.cq:10:11..14
      value: 0
    )
    ConstInt( # res/v1x/grover.cq:10:11..14
      value: 1
    )
    ConstInt( # res/v1x/grover.cq:10:11..14
      value: 2
    )
    ConstInt( # res/v1x/grover.cq:10:11..14
      value: 3
    )
    ConstInt(
      value: 6
    )
  ]
)
)");

    // Notice that the tree stores where the nodes that we didn't make ourselves
    // in this test case came from. This is accomplished using annotations,
    // of the type cq1x::parser::SourceLocation in this case:
    EXPECT_EQ(clone->get_annotation<cq1x::parser::SourceLocation>().filename, "res/v1x/grover.cq");

    // Annotations are a big magical in C++ land, in that you can annotate nodes
    // with *any* C++ object of *any* type, without having to modify libqasm!
    // The source location annotation is only special in that libqasm adds it
    // on its own, and prints the corresponding location in the debug dumps if
    // one is defined. For example, we could define a type real quick...
    struct MyAnnotationType {
        int number;
    };
    // ... and add it to the QubitRefs node.
    EXPECT_EQ(clone->has_annotation<MyAnnotationType>(), false);
    EXPECT_EQ(clone->get_annotation_ptr<MyAnnotationType>(), nullptr);
    EXPECT_THROW(clone->get_annotation<MyAnnotationType>().number, std::runtime_error);
    clone->set_annotation(MyAnnotationType {42});
    EXPECT_EQ(clone->has_annotation<MyAnnotationType>(), true);
    EXPECT_EQ(clone->get_annotation_ptr<MyAnnotationType>()->number, 42);
    EXPECT_EQ(clone->get_annotation<MyAnnotationType>().number, 42);

    // It's not just a void* wrapped in some casting either; you can in fact
    // have zero or one annotation of *every* C++ type *at the same time*.
    // So the source location is still there:
    EXPECT_EQ(clone->get_annotation<cq1x::parser::SourceLocation>().filename, "res/v1x/grover.cq");

    // When you call set_annotation, your object is either moved or copied into
    // the underlying annotation map. If you have something that cannot be moved
    // or copied, or you want to keep a mutable reference to it, you can wrap
    // your object in a (smart) pointer of some kind.
    //
    // While libqasm handles cleanup for you (since it owns the copy of your
    // object), it can't copy your object after the initial copy/move. That
    // might be a bit unexpected given the fact that clone() exists, but it's
    // extremely hard to avoid due to all the magic. clone() instead copies
    // *references to* the originally copied/moved object.
    //
    // Avoid using primitive types as annotations. It's legal, but when you're
    // attaching an int to some node it's not exactly obvious from context what
    // the int is for, and another piece of code might use the same type and
    // thus conflict. That's why we used a struct just now, but any type will
    // do.

    // That's pretty much it for basic usage of this API and its trees. At the
    // time of writing there is no further documentation other than the
    // docstrings in the code, so you might want to get an IDE with a C++ index
    // when developing stuff using libqasm if you aren't using one already.
    // Good luck!

}

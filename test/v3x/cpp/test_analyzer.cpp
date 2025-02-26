#include <gmock/gmock.h>

#include <functional>

#include "libqasm/error.hpp"
#include "libqasm/tree.hpp"
#include "libqasm/v3x/analyzer.hpp"
#include "libqasm/v3x/syntactic.hpp"
#include "libqasm/v3x/parse_result.hpp"
#include "libqasm/version.hpp"
#include "mock_analyzer.hpp"

using namespace ::testing;

namespace cqasm::v3x::analyzer {

//---------------------//
// AnalyzerAnalyzeTest //
//---------------------//

class AnalyzerAnalyzeTest : public ::testing::Test {
protected:
    void SetUp() override {}

    version::Version version_3_0 = "3.0";

    tree::One<syntactic::Version> version = tree::make<syntactic::Version>(version_3_0);
    tree::One<syntactic::GlobalBlock> global_block = tree::make<syntactic::GlobalBlock>();
    tree::One<syntactic::Program> program = tree::make<syntactic::Program>(version, global_block);
    parser::ParseResult parse_result_ok = parser::ParseResult{ program, error::ParseErrors{} };

    std::string parse_error_message{ "parse error" };
    error::ParseErrors parse_errors{ error::ParseError{ parse_error_message } };
    parser::ParseResult parse_result_errors = parser::ParseResult{ tree::make<syntactic::Program>(), parse_errors };
};

TEST_F(AnalyzerAnalyzeTest, parser_returns_parse_result) {
    auto analyzer = Analyzer{};
    const auto& analysis_result = analyzer.analyze(parse_result_ok);
    auto program = analysis_result.root->as_program();
    const auto& version = program->version->items;
    EXPECT_EQ(version, version_3_0);
}
TEST_F(AnalyzerAnalyzeTest, parser_returns_errors) {
    auto analyzer = Analyzer{};
    const auto& analysis_result = analyzer.analyze(parse_result_errors);
    const auto& error = analysis_result.errors[0];
    EXPECT_THAT(error.what(), ::testing::HasSubstr(parse_error_message));
}

//--------------//
// AnalyzerTest //
//--------------//

class AnalyzerTest : public ::testing::Test {
protected:
    void SetUp() override {}

    instruction::InstructionRef instruction_ref = tree::make<instruction::Instruction>("h", "Q");
    tree::One<semantic::Variable> qubit = tree::make<semantic::Variable>("q", tree::make<types::Qubit>());
    tree::Link<semantic::Variable> qubit_link{ qubit };
    tree::One<values::VariableRef> qubit_variable_ref = tree::make<values::VariableRef>(qubit_link);
    tree::Any<values::ValueBase> operands{ qubit_variable_ref };
    tree::One<semantic::Gate> gate = tree::make<semantic::Gate>(instruction_ref->name);
    tree::One<semantic::Instruction> statement = tree::make<semantic::GateInstruction>(instruction_ref, gate, operands);
};

TEST_F(AnalyzerTest, constructor) {
    MockAnalyzer analyzer{};
    EXPECT_EQ(analyzer.scope_stack().size(), 1);
    EXPECT_FALSE(analyzer.current_block().empty());
    EXPECT_TRUE(analyzer.current_variables().empty());
}
TEST_F(AnalyzerTest, push_scope) {
    MockAnalyzer analyzer{};
    analyzer.push_scope();
    EXPECT_EQ(analyzer.scope_stack().size(), 2);
}
TEST_F(AnalyzerTest, pop_scope) {
    MockAnalyzer analyzer{};
    analyzer.pop_scope();
    EXPECT_TRUE(analyzer.scope_stack().empty());
}
TEST_F(AnalyzerTest, add_statement_to_current_scope) {
    MockAnalyzer analyzer{};
    analyzer.add_statement_to_current_scope(statement);
    EXPECT_EQ(analyzer.current_block()->statements.size(), 1);
}
TEST_F(AnalyzerTest, add_statement_with_source_location_information_to_current_scope) {
    MockAnalyzer analyzer{};
    const auto& statement_source_location = annotations::SourceLocation{
        "input.cq", { { 10, 20 }, { 11, 10 } }
    };
    statement->set_annotation(statement_source_location);
    analyzer.add_statement_to_current_scope(statement);
    EXPECT_EQ(analyzer.current_block()->statements.size(), 1);
    const auto& block_source_location = analyzer.current_block()->get_annotation<annotations::SourceLocation>();
    EXPECT_EQ(block_source_location.file_name, statement_source_location.file_name);
    EXPECT_EQ(block_source_location.range, statement_source_location.range);
}
TEST_F(AnalyzerTest,
    add_statement_with_source_location_information_to_current_scope_and_block_has_source_location_information) {
    MockAnalyzer analyzer{};
    //     10 15 20 25 30
    //  5      <
    //  8               >
    const auto& block_initial_source_location = annotations::SourceLocation{
        "input.cq", { { 5, 15 }, { 8, 30 } }
    };
    analyzer.current_block()->set_annotation(block_initial_source_location);
    //     10 15 20 25 30
    // 10   <
    // 11         >
    const auto& statement_source_location = annotations::SourceLocation{
        "input.cq", { { 10, 10 }, { 11, 20 } }
    };
    statement->set_annotation(statement_source_location);
    analyzer.add_statement_to_current_scope(statement);
    EXPECT_EQ(analyzer.current_block()->statements.size(), 1);
    //     10 15 20 25 30
    //  5      <
    // 11         >
    const auto& block_final_source_location = analyzer.current_block()->get_annotation<annotations::SourceLocation>();
    EXPECT_EQ(block_final_source_location.file_name, "input.cq");
    EXPECT_EQ(block_final_source_location.range,
        (annotations::SourceLocation::Range{
            {  5, 15 },
            { 11, 20 }
    }));
}

}  // namespace cqasm::v3x::analyzer

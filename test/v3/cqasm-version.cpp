#include "cqasm-version.hpp"
#include "cqasm-error.hpp"

#include <cstdio>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace ::testing;
using namespace cqasm::error;
using namespace cqasm::version;


TEST(parse_file, filename_does_not_exist) {
    EXPECT_THROW(parse_file("res/cqasm_version/does_not_exist.cq"), AnalysisError);
}
TEST(parse_file, filename_empty) {
    EXPECT_THROW(parse_file("res/cqasm_version/empty.cq"), AnalysisError);
}
TEST(parse_file, filename_version_no_number) {
    EXPECT_THROW(parse_file("res/cqasm_version/version_no_number.cq"), AnalysisError);
}
TEST(parse_file, filename_version_abc) {
    EXPECT_THROW(parse_file("res/cqasm_version/version_abc.cq"), AnalysisError);
}
TEST(parse_file, filename_version_1_abc) {
    EXPECT_THROW(parse_file("res/cqasm_version/version_1_abc.cq"), AnalysisError);
}
TEST(parse_file, filename_version_1_1_abc) {
    EXPECT_THROW(parse_file("res/cqasm_version/version_1_1_abc.cq"), AnalysisError);
}
TEST(parse_file, filename_version_1_0) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_1_0.cq"), Version{ "1.0" });
}
TEST(parse_file, filename_version_1_1) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_1_1.cq"), Version{ "1.1" });
}
TEST(parse_file, filename_version_1_1_1) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_1_1_1.cq"), Version{ "1.1.1" });
}
TEST(parse_file, filename_version_1_2) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_1_2.cq"), Version{ "1.2" });
}
TEST(parse_file, filename_version_1_3) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_1_3.cq"), Version{ "1.3" });
}
TEST(parse_file, filename_version_2_0) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_2_0.cq"), Version{ "2.0" });
}
TEST(parse_file, filename_version_3_0) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_3_0.cq"), Version{ "3.0" });
}
TEST(parse_file, filename_version_4_0) {
    EXPECT_EQ(parse_file("res/cqasm_version/version_4_0.cq"), Version{ "4.0" });
}


TEST(parse_file, fp_does_not_exist) {
    const char *filename{ "res/cqasm_version/does_not_exist.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_THROW(parse_file(fp, filename), AnalysisError);
}
TEST(parse_file, fp_empty) {
    const char *filename{ "res/cqasm_version/empty.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_THROW(parse_file(fp, filename), AnalysisError);
}
TEST(parse_file, fp_version_no_number) {
    const char *filename{ "res/cqasm_version/version_no_number.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_THROW(parse_file(fp, filename), AnalysisError);
}
TEST(parse_file, fp_version_abc) {
    const char *filename{ "res/cqasm_version/version_abc.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_THROW(parse_file(fp, filename), AnalysisError);
}
TEST(parse_file, fp_version_1_abc) {
    const char *filename{ "res/cqasm_version/version_1_abc.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_THROW(parse_file(fp, filename), AnalysisError);
}
TEST(parse_file, fp_version_1_1_abc) {
    const char *filename{ "res/cqasm_version/version_1_1_abc.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_THROW(parse_file(fp, filename), AnalysisError);
}
TEST(parse_file, fp_version_1_0) {
    const char *filename{ "res/cqasm_version/version_1_0.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "1.0" });
}
TEST(parse_file, fp_version_1_1) {
    const char *filename{ "res/cqasm_version/version_1_1.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "1.1" });
}
TEST(parse_file, fp_version_1_1_1) {
    const char *filename{ "res/cqasm_version/version_1_1_1.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "1.1.1" });
}
TEST(parse_file, fp_version_1_2) {
    const char *filename{ "res/cqasm_version/version_1_2.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "1.2" });
}
TEST(parse_file, fp_version_1_3) {
    const char *filename{ "res/cqasm_version/version_1_3.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "1.3" });
}
TEST(parse_file, fp_version_2_0) {
    const char *filename{ "res/cqasm_version/version_2_0.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "2.0" });
}
TEST(parse_file, fp_version_3_0) {
    const char *filename{ "res/cqasm_version/version_3_0.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "3.0" });
}
TEST(parse_file, fp_version_4_0) {
    const char *filename{ "res/cqasm_version/version_4_0.cq" };
    FILE *fp{ fopen(filename, "r") };
    EXPECT_EQ(parse_file(fp, filename), Version{ "4.0" });
}


TEST(parse_string, string_empty) {
    const char *filename{ "res/cqasm_version/empty.cq" };
    EXPECT_THROW(parse_string("", filename), AnalysisError);
}
TEST(parse_string, string_version_no_number) {
    const char *filename{ "res/cqasm_version/version_no_number.cq" };
    EXPECT_THROW(parse_string("version", filename), AnalysisError);
}
TEST(parse_string, string_version_abc) {
    const char *filename{ "res/cqasm_version/version_abc.cq" };
    EXPECT_THROW(parse_string("version abc", filename), AnalysisError);
}
TEST(parse_string, string_version_1_abc) {
    const char *filename{ "res/cqasm_version/version_1_abc.cq" };
    EXPECT_THROW(parse_string("version 1.abc", filename), AnalysisError);
}
TEST(parse_string, string_version_1_1_abc) {
    const char *filename{ "res/cqasm_version/version_1_1_abc.cq" };
    EXPECT_THROW(parse_string("version 1.1.abc", filename), AnalysisError);
}
TEST(parse_string, string_version_1_0) {
    const char *filename{ "res/cqasm_version/version_1_0.cq" };
    EXPECT_EQ(parse_string("version 1.0", filename), Version{ "1.0" });
}
TEST(parse_string, string_version_1_1) {
    const char *filename{ "res/cqasm_version/version_1_1.cq" };
    EXPECT_EQ(parse_string("version 1.1", filename), Version{ "1.1" });
}
TEST(parse_string, string_version_1_1_1) {
    const char *filename{ "res/cqasm_version/version_1_1_1.cq" };
    EXPECT_EQ(parse_string("version 1.1.1", filename), Version{ "1.1.1" });
}
TEST(parse_string, string_version_1_2) {
    const char *filename{ "res/cqasm_version/version_1_2.cq" };
    EXPECT_EQ(parse_string("version 1.2", filename), Version{ "1.2" });
}
TEST(parse_string, string_version_1_3) {
    const char *filename{ "res/cqasm_version/version_1_3.cq" };
    EXPECT_EQ(parse_string("version 1.3", filename), Version{ "1.3" });
}
TEST(parse_string, string_version_2_0) {
    const char *filename{ "res/cqasm_version/version_2_0.cq" };
    EXPECT_EQ(parse_string("version 2.0", filename), Version{ "2.0" });
}
TEST(parse_string, string_version_3_0) {
    const char *filename{ "res/cqasm_version/version_3_0.cq" };
    EXPECT_EQ(parse_string("version 3.0", filename), Version{ "3.0" });
}
TEST(parse_string, string_version_4_0) {
    const char *filename{ "res/cqasm_version/version_4_0.cq" };
    EXPECT_EQ(parse_string("version 4.0", filename), Version{ "4.0" });
}


struct ScannerMock : public ScannerAdaptor {
    MOCK_METHOD((int), parse, (const std::string &, Version &), (const, override));
};

TEST(ParseHelper_parse, scanner_returns_2) {
    auto scanner_up = std::unique_ptr<ScannerMock>(new ScannerMock());
    const auto &scanner = *(dynamic_cast<ScannerMock*>(scanner_up.get()));
    const std::string filename = "huge_file.cq";
    EXPECT_CALL(scanner, parse(filename, _)).WillOnce(Return(2));
    EXPECT_THROW(ParseHelper(filename, std::move(scanner_up)).parse(), AnalysisError);
}
TEST(ParseHelper_parse, scanner_returns_10) {
    auto scanner_up = std::unique_ptr<ScannerMock>(new ScannerMock);
    const auto &scanner = *(dynamic_cast<ScannerMock*>(scanner_up.get()));
    const std::string filename = "wrong_file.cq";
    EXPECT_CALL(scanner, parse(filename, _)).WillOnce(Return(10));
    EXPECT_THROW(ParseHelper(filename, std::move(scanner_up)).parse(), AnalysisError);
}
TEST(ParseHelper_parse, scanner_returns_0_and_version_is_empty) {
    auto scanner_up = std::unique_ptr<ScannerMock>(new ScannerMock);
    const auto &scanner = *(dynamic_cast<ScannerMock*>(scanner_up.get()));
    const std::string filename = "file_no_version.cq";
    EXPECT_CALL(scanner, parse(filename, _)).WillOnce(Return(0));
    EXPECT_THROW(ParseHelper(filename, std::move(scanner_up)).parse(), AnalysisError);
}
TEST(ParseHelper_parse, scanner_returns_0_and_version_is_ok) {
    auto scanner_up = std::unique_ptr<ScannerMock>(new ScannerMock);
    const auto &scanner = *(dynamic_cast<ScannerMock*>(scanner_up.get()));
    const std::string filename = "file_version_3_0.cq";
    Version ret{ "3.0" };
    EXPECT_CALL(scanner, parse(filename, _))
        .WillOnce(DoAll(SetArgReferee<1>(ret), Return(0)));
    EXPECT_EQ(ParseHelper(filename, std::move(scanner_up)).parse(), Version{ "3.0" });
}

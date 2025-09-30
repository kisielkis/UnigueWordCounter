#include "UniqueWordCounter.h"

#include <filesystem>
#include <gtest/gtest.h>
#include <sstream>

namespace fs = std::filesystem;

const static fs::path programPath = "./program";
const static std::string testDataPath = "test_data/";

TEST(IntegrationTtest, FourWord) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;

    std::string parameter = testDataPath + "test1.txt";
    std::string command = programPath.string() + " " + parameter;

    int ret = std::system(command.c_str());
    int exitCode = WEXITSTATUS(ret);

    EXPECT_EQ(exitCode, 4);
}

TEST(IntegrationTtest, OneWord) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;

    std::string parameter = testDataPath + "test2.txt";
    std::string command = programPath.string() + " " + parameter;

    int ret = std::system(command.c_str());
    int exitCode = WEXITSTATUS(ret);

    EXPECT_EQ(exitCode, 1);
}

TEST(IntegrationTtest, EmptyFile) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;

    std::string parameter = testDataPath + "test3.txt";
    std::string command = programPath.string() + " " + parameter;

    int ret = std::system(command.c_str());
    int exitCode = WEXITSTATUS(ret);

    EXPECT_EQ(exitCode, 0);
}

TEST(IntegrationTtest, OneWordBigFile) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;

    std::string parameter = testDataPath + "test5.txt";
    std::string command = programPath.string() + " " + parameter;

    int ret = std::system(command.c_str());
    int exitCode = WEXITSTATUS(ret);

    EXPECT_EQ(exitCode, 1);
}


TEST(IntegrationTtest, nineTeenWord) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;

    std::string parameter = testDataPath + "test6.txt";
    std::string command = programPath.string() + " " + parameter;

    int ret = std::system(command.c_str());
    int exitCode = WEXITSTATUS(ret);

    EXPECT_EQ(exitCode, 19);
}

TEST(IntegrationTtest, nineTeenWordBigFile) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;

    std::string parameter = testDataPath + "test7.txt";
    std::string command = programPath.string() + " " + parameter;

    int ret = std::system(command.c_str());
    int exitCode = WEXITSTATUS(ret);

    EXPECT_EQ(exitCode, 19);
}

TEST(IntegrationTtest, FileNotExist) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;

    std::string parameter = testDataPath + "none.txt";
    std::string command = programPath.string() + " " + parameter;

    int ret = std::system(command.c_str());
    int exitCode = WEXITSTATUS(ret);

    EXPECT_EQ(exitCode, 0);
}
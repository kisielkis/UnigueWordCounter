#include "UniqueWordCounter.h"

#include <filesystem>
#include <gtest/gtest.h>
#include <sstream>

namespace fs = std::filesystem;

const static fs::path programPath = "./program";
const static std::string testDataPath = "test_data/";

static int runAndReadCount(const std::string& parameter) {
    std::string command = programPath.string() + " " + parameter;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return -1;
    char buffer[32] = {};
    fgets(buffer, sizeof(buffer), pipe);
    pclose(pipe);
    return std::stoi(buffer);
}

TEST(IntegrationTtest, FourWord) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;
    EXPECT_EQ(runAndReadCount(testDataPath + "test1.txt"), 4);
}

TEST(IntegrationTtest, OneWord) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;
    EXPECT_EQ(runAndReadCount(testDataPath + "test2.txt"), 1);
}

TEST(IntegrationTtest, EmptyFile) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;
    EXPECT_EQ(runAndReadCount(testDataPath + "test3.txt"), 0);
}

TEST(IntegrationTtest, OneWordBigFile) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;
    EXPECT_EQ(runAndReadCount(testDataPath + "test5.txt"), 1);
}

TEST(IntegrationTtest, nineTeenWord) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;
    EXPECT_EQ(runAndReadCount(testDataPath + "test6.txt"), 19);
}

TEST(IntegrationTtest, nineTeenWordBigFile) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;
    EXPECT_EQ(runAndReadCount(testDataPath + "test7.txt"), 19);
}

TEST(IntegrationTtest, FileNotExist) {
    ASSERT_TRUE(fs::exists(programPath)) << "Program do not exist " << programPath;
    EXPECT_EQ(runAndReadCount(testDataPath + "none.txt"), 0);
}

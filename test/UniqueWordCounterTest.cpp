#include "UniqueWordCounter.h"

#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <unistd.h>

size_t countUniqueWordsInString(const std::string& input) {
    std::istringstream iss(input);
    std::string word;
    std::unordered_set<std::string> uniqueWords;
    while (iss >> word) {
        uniqueWords.insert(word);
    }
    return uniqueWords.size();
}

TEST(UniqueWordCounterTest, HandlesRepeatingWords) {
    std::string input = "cat cat cat cat";
    EXPECT_EQ(countUniqueWordsInString(input), 1);
}

TEST(UniqueWordCounterTest, HandlesSingleWord) {
    std::string input = "dog";
    EXPECT_EQ(countUniqueWordsInString(input), 1);
}

TEST(UniqueWordCounterTest, HandlesMultipleUniqueWords) {
    std::string input = "a horse and a dog";
    EXPECT_EQ(countUniqueWordsInString(input), 4);
}

TEST(UniqueWordCounterTest, HandlesEmptyInput) {
    std::string input = "";
    EXPECT_EQ(countUniqueWordsInString(input), 0);
}

TEST(UniqueWordCounterTest, HandlesLongInput) {
    std::string input = "the quick brown fox jumps over the lazy dog";
    EXPECT_EQ(countUniqueWordsInString(input), 8);
}

TEST(UniqueWordCounterTest, HandlesLargeNumberOfRepeatingWords) {
    std::string input(1000000, 'a'); // 1 milion 'a'
    input += " ";
    EXPECT_EQ(countUniqueWordsInString(input), 1);
}

TEST(UniqueWordCounterTest, HandlesLargeNumberOfUniqueWords) {
    std::string input;
    for (int i = 0; i < 100000; ++i) {
        input += "word" + std::to_string(i) + " ";
    }
    EXPECT_EQ(countUniqueWordsInString(input), 100000);
}

// TEST(UniqueWordCounterTest, HandlesLargeMixedInput) {
//     std::string input;
//     for (int i = 0; i < 5000000; ++i) {
//         input += "repeat ";
//     }
//     for (int i = 0; i < 9000000; ++i) {
//         input += "unique" + std::to_string(i) + " ";
//     }
//     EXPECT_EQ(countUniqueWordsInString(input), 9000001);
// }

// ---------------------------------------------------------------------------
// Tests for the UniqueWordCounter class itself (Bug 4 fix)
// ---------------------------------------------------------------------------

class UniqueWordCounterClassTest : public ::testing::Test {
protected:
    char tmpPath[64] = "/tmp/uwc_test_XXXXXX";
    int fd = -1;

    void SetUp() override {
        fd = mkstemp(tmpPath);
        ASSERT_NE(fd, -1) << "mkstemp failed";
        close(fd);
    }

    void TearDown() override {
        unlink(tmpPath);
    }

    void writeFile(const std::string& content) {
        std::ofstream ofs(tmpPath, std::ios::binary);
        ofs << content;
    }

    size_t count() {
        UniqueWordCounter counter(tmpPath);
        return counter.countUniqueWords();
    }
};

TEST_F(UniqueWordCounterClassTest, EmptyFile) {
    writeFile("");
    EXPECT_EQ(count(), 0u);
}

TEST_F(UniqueWordCounterClassTest, SingleWord) {
    writeFile("dog");
    EXPECT_EQ(count(), 1u);
}

TEST_F(UniqueWordCounterClassTest, RepeatedWords) {
    writeFile("cat cat cat cat");
    EXPECT_EQ(count(), 1u);
}

TEST_F(UniqueWordCounterClassTest, MultipleUniqueWords) {
    writeFile("a horse and a dog");
    EXPECT_EQ(count(), 4u);
}

TEST_F(UniqueWordCounterClassTest, WordsSeparatedByNewline) {
    writeFile("hello\nworld\nhello");
    EXPECT_EQ(count(), 2u);
}

TEST_F(UniqueWordCounterClassTest, WordsSeparatedByCRLF) {
    writeFile("hello\r\nworld\r\nhello");
    EXPECT_EQ(count(), 2u);
}

TEST_F(UniqueWordCounterClassTest, NonExistentFile) {
    UniqueWordCounter counter("/tmp/this_file_does_not_exist_uwc");
    EXPECT_EQ(counter.countUniqueWords(), 0u);
}

TEST_F(UniqueWordCounterClassTest, LargeFileRepeatingWords) {
    std::string content;
    content.reserve(700000);
    for (int i = 0; i < 100000; ++i) content += "hello ";
    writeFile(content);
    EXPECT_EQ(count(), 1u);
}

TEST_F(UniqueWordCounterClassTest, LargeFileUniqueWords) {
    std::string content;
    for (int i = 0; i < 10000; ++i) content += "word" + std::to_string(i) + " ";
    writeFile(content);
    EXPECT_EQ(count(), 10000u);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

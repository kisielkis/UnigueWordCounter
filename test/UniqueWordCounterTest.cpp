#include "UniqueWordCounter.h"

#include <gtest/gtest.h>
#include <sstream>

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

TEST(UniqueWordCounterTest, HandlesLargeMixedInput) {
    std::string input;
    for (int i = 0; i < 5000000; ++i) {
        input += "repeat ";
    }
    for (int i = 0; i < 9000000; ++i) {
        input += "unique" + std::to_string(i) + " ";
    }
    EXPECT_EQ(countUniqueWordsInString(input), 9000001);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

 /**
  * @file UniqueWordCounter.h
  * @author Marcin Kisielewski kisielkis@wp.pl
  * @date 2/09/2025
  * 
  */

#ifndef UNIQUE_WORD_COUNTER_H
#define UNIQUE_WORD_COUNTER_H

#include <string>
#include <unordered_set>
#include <vector>
#include <thread>
#include <mutex>

/**
 * @class UniqueWordCounter
 * @brief A class to count unique words in a file using multithreading.
 */
class UniqueWordCounter {
public:
    /**
     * @brief Constructor that initializes with a filename.
     * @param filename The name of the file to process.
     */
    UniqueWordCounter(const std::string& filename);

    /**
     * @brief Counts the number of unique words in the file.
     * @return The count of unique words.
     */
    size_t countUniqueWords();

private:
    std::string filename_;
    const char* file_content_ = nullptr;
    size_t file_size_ = 0;
    std::vector<std::thread> threads_;
    std::unordered_set<std::string> unique_words_;
    std::mutex mtx_; 

    /**
     * @brief Maps the file into memory.
     * @return True if successful, false otherwise.
     */
    bool mapFile();

    /**
     * @brief Unmaps the file from memory.
     */
    void unmapFile();

    /**
     * @brief Processes a chunk of the file to extract unique words.
     * @param start Pointer to the start of the chunk.
     * @param end Pointer to the end of the chunk.
     */
    void processChunk(const char* start, const char* end);
};  // end class UniqueWordCounter

#endif // end UNIQUE_WORD_COUNTER_H

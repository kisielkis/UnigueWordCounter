#include "UniqueWordCounter.h"

#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

UniqueWordCounter::UniqueWordCounter(const std::string& filename) : filename_(filename) {}

UniqueWordCounter::~UniqueWordCounter() {
    for (auto& [t, s] : threads_and_count_) {
        if (t.joinable()) t.join();
    }
    unmapFile();
}

size_t UniqueWordCounter::countUniqueWords() {
    for (auto& [t, s] : threads_and_count_) {
        if (t.joinable()) t.join();
    }
    threads_and_count_.clear();
    unique_words_.clear();

    if (!mapFile()) {
        std::cerr << "Error mapping file" << std::endl;
        return 0;
    }

    if (file_size_ == 0) {
        return 0;
    }

    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
        num_threads = 1;
    }
    size_t chunk_size = file_size_ / num_threads;

    threads_and_count_.reserve(num_threads);

    for (size_t iTer = 0; iTer < num_threads; ++iTer) {
        const char* start = file_content_ + iTer * chunk_size;
        const char* end = (iTer == num_threads - 1) ? file_content_ + file_size_ : start + chunk_size;

        auto isDelimiter = [](char c) { return c == ' ' || c == '\n' || c == '\r'; };

        // Adjust start: skip past any partial word already covered by the previous chunk
        if (iTer != 0) {
            while (start < file_content_ + file_size_ && !isDelimiter(*start)) {
                ++start;
            }

            while (start < file_content_ + file_size_ && isDelimiter(*start)) {
                ++start;
            }
            if (start >= file_content_ + file_size_) {
                break;
            }
        }

        // Ensure we don't split a word between chunks
        if (iTer != num_threads - 1) {
            while (end < file_content_ + file_size_ && !isDelimiter(*end)) {
                ++end;
            }
        }

        threads_and_count_.emplace_back();
        threads_and_count_.back().second.reserve(chunk_size / 5);
        threads_and_count_.back().first = std::thread(
            &UniqueWordCounter::processChunk, this, start, end,
            &threads_and_count_.back().second 
        );
    }

    for (auto& [t, s] : threads_and_count_) t.join();

    size_t total_estimate = 0;
    for (auto& [t, s] : threads_and_count_) total_estimate += s.size();
    unique_words_.reserve(total_estimate);

    for (auto& [t, s] : threads_and_count_) unique_words_.merge(s);

    unmapFile();
    return unique_words_.size();
}

bool UniqueWordCounter::mapFile() {
    int fd = open(filename_.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening file" << std::endl;
        return false;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "Error getting file size" << std::endl;
        close(fd);
        return false;
    }

    file_size_ = sb.st_size;

    if (file_size_ == 0) {
        close(fd);
        return true;
    }

    /*
     * alternatively: buffered ifstream or boost::iostreams::mapped_file_source
     */
    file_content_ = static_cast<char*>(mmap(nullptr, file_size_, PROT_READ, MAP_PRIVATE, fd, 0));
    close(fd);
    return file_content_ != MAP_FAILED;
}

void UniqueWordCounter::unmapFile() {
    if (file_content_) {
        munmap(file_content_, file_size_);
        file_content_ = nullptr;
    }
}

void UniqueWordCounter::processChunk(const char* start, const char* end, std::unordered_set<std::string>* result) {
    const char* word_start = nullptr;
    for (const char* it = start; it < end; ++it) {
        if (*it == ' ' || *it == '\n' || *it == '\r') {
            if (word_start) {
                result->emplace(word_start, static_cast<size_t>(it - word_start));
                word_start = nullptr;
            }
        } else if (!word_start) {
            word_start = it;
        }
    }
    // Flush last word if chunk doesn't end with a delimiter
    if (word_start) {
        result->emplace(word_start, static_cast<size_t>(end - word_start));
    }
}

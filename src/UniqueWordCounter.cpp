#include "UniqueWordCounter.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

UniqueWordCounter::UniqueWordCounter(const std::string& filename) : filename_(filename) {}

size_t UniqueWordCounter::countUniqueWords() {
    if (!mapFile()) {
        std::cerr << "Error mapping file" << std::endl;
        return 0;
    }

    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
        num_threads = 1;
    }
    size_t chunk_size = file_size_ / num_threads;

    for (size_t iTer = 0; iTer < num_threads; ++iTer) {
        const char* start = file_content_ + iTer * chunk_size;
        const char* end = (iTer == num_threads - 1) ? file_content_ + file_size_ : start + chunk_size;

        // Adjust start to the beginning of a word
        if (iTer != 0) {
            while (start < file_content_ + file_size_ && *start != ' ') {
                ++start;
            }
            if (start >= file_content_ + file_size_) {
                break;
            }
        }

        // Ensure we don't split a word between chunks
        if (iTer != num_threads - 1) {
            while (end < file_content_ + file_size_ && *end != ' ') {
                ++end;
            }
        }
        threads_.emplace_back(&UniqueWordCounter::processChunk, this, start, end);
    }

    for (auto& thread : threads_) {
        thread.join();
    }

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
    /*
     * alternatively: buffered ifstream or boost::iostreams::mapped_file_source
     */
    file_content_ = static_cast<const char*>(mmap(nullptr, file_size_, PROT_READ, MAP_PRIVATE, fd, 0));
    close(fd);
    return file_content_ != MAP_FAILED;
}

void UniqueWordCounter::unmapFile() {
    if (file_content_) {
        munmap(const_cast<char*>(file_content_), file_size_);
    }
}

void UniqueWordCounter::processChunk(const char* start, const char* end) {
    std::string word;
    for (const char* it = start; it < end; ++it) {
        if (*it == ' ') {
            if (!word.empty()) {
                std::lock_guard<std::mutex> lock(mtx_);
                unique_words_.insert(word);
                word.clear();
            }
        } else {
            word += *it;
        }
    }
    if (!word.empty()) {
        std::lock_guard<std::mutex> lock(mtx_);
        unique_words_.insert(word);
    }
}

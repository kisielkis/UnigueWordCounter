/** 
 * Please submit the complete C++ source code for a program that counts the number of distinct 
 * unique words in a file whose name is passed as an argument to a program. For example, given 
 * the file content "a horse and a dog" the program must output "4" (the word 'a' appears twice 
 * but only accounts for one distinct unique occurrence). The input text is guaranteed to contain 
 * only 'a'..'z' and space characters in ASCII encoding.  
 * You can assume that all unique words fit into memory when using the data structure of your choice.
 * The program should be able to handle large inputs, much larger than the amount of available RAM (e.g. 32 GiB).  
 * The solution should be multithreaded, use only C/C++ standard library and/or Linux syscalls.
 */

#include <iostream>

#include "UniqueWordCounter.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    UniqueWordCounter counter(argv[1]);
    size_t unique_word_count = counter.countUniqueWords();
    std::cout << unique_word_count << std::endl;

    return unique_word_count;
}

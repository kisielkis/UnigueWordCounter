# Unique Word Counter

## 📖 Description
This project is a **multithreaded C++ program** that counts the number of **distinct unique words** in a text file.  
The program takes the input filename as a command-line argument.

### Example
For a file containing: 

> a horse and a dog

the program outputs: 4

## ⚙️ Assumptions
- Input file contains only characters `a`..`z` and spaces in ASCII encoding.  
- The program handles **very large files** (e.g., 32 GiB), much larger than available RAM.  
- All unique words fit into memory (e.g., stored in a set-like structure).  
- Implementation is **multithreaded** to utilize multiple CPU cores.  
- Only C/C++ standard libraries and Linux syscalls are used.  

---

## ✨ Features
- Efficient processing of large files by reading them in blocks  
- Parallel word counting with multithreading  
- Memory-friendly design (memory usage depends only on number of unique words, not file size)  
- Portable C++17 code with Linux syscall support  

---

## 🛠️ Build
make

## ✅ Test
```
make test
```

## 🚀 Run
```
./program file.txt
```
## ⚡ Run test
```
./test_program
```

## 📝 TODO
- repleace one Mutex to increase program speed

# Word Counter — C Hash Table Implementation

A word-frequency counter written in C using a custom hash table with separate chaining and tail pointers for optimized insertions.

## Overview

This project reads text from a input file (`lines.txt`), tokenizes individual words, converts them to lowercase, and tracks their frequencies in a custom hash table.

## Features

- **Tail Pointer Optimization:** Each bucket maintains `start` and `end` pointers (`struct POINTERS`), enabling $O(1)$ insertions at the end of collision lists.
- **Custom Tokenizer:** Extracts alphabetical characters and normalizes upper-case characters to lower-case during stream reading.
- **Performance Timing:** Measures program execution time using `<time.h>`.

## Data Structures

```c
// Node representing a unique word and its frequency
struct NODE {
    int cnt;        // Word frequency count
    char word[60];  // String content
    struct NODE* next;
};

// Bucket entry maintaining head and tail pointers
struct POINTERS {
    word *start;
    word *end;
};
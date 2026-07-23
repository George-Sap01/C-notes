# Binary File Record Manager in C

A menu-driven C application designed to experiment with direct binary file manipulation, record-based file I/O, dynamic pointer offset navigation using `fseek`, and binary record swapping.

---

## Overview

Working with files in C often starts with plain text (`.txt`), but binary files (`.dat`) allow direct, efficient storage of contiguous C structures (`struct`). 

This project explores low-level binary file handling by implementing a custom record manager. Records are stored sequentially on disk along with a header offset that tracks the total number of active records.

---

## Key Features & Techniques

* **Header-Driven File Layout:** The binary file reserves its first 4 bytes (`sizeof(int)`) to store the current record count (`n`), followed directly by contiguous `struct record` blocks.
* **Direct File Seeking (`fseek` & `rewind`):** Uses precise byte offset calculations (`4 + sizeof(struct record) * index`) to jump straight to specific records without reading the entire file into memory.
* **Efficient $O(1)$ Record Deletion:** Implements a swap-and-truncate approach—deleting a record overwrites it with the last record in the file and decrements the global header count.
* **In-Place Record Swapping:** Swaps positions of any two records directly on disk using temporary `struct` buffers and targeted file writes.
* **Interactive CLI Interface:** Fully menu-driven system supporting full CRUD operations (Create, Read, Update, Delete) plus position reordering.

---

## Binary File Architecture

```
Byte Offset: 0         4                    4 + sizeof(record)   ...
             +---------+--------------------+--------------------+
             | int (n) | record 1 (name,age)| record 2 (name,age)| ...
             +---------+--------------------+--------------------+
```

---

## Operations Supported

1. **Open File (`open_file`):** Safely opens or initializes `DATA.dat` in `rb+` / `wb+` mode.
2. **Close File (`close_file`):** Gracefully closes the active file handle.
3. **Show Deposits (`num_deposits`):** Reads the total record count stored at byte 0.
4. **Add Record (`add_record`):** Appends a new `struct record` at offset `4 + sizeof(record) * n` and increments `n`.
5. **Print Record (`print_record`):** Fetches and displays a specific record by index.
6. **Print All Records (`print_all`):** Sequentially reads and formats all stored records.
7. **Modify Record (`modify_record`):** Overwrites fields of an existing record in-place.
8. **Delete Record (`delete_record`):** Replaces targeted record with the tail record and updates count.
9. **Change Position (`change_pos`):** Swaps two record locations directly on disk.

---

## Struct Definition

```c
struct record {
    char name[80];
    int age;
    // Extensible for additional attributes...
};
```

---
## License

This project is intended purely for educational and experimental purposes.
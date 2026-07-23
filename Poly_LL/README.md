# Polymorphic Heterogeneous Linked List in C11

An experimental C project demonstrating compile-time polymorphism and safe handling of heterogeneous data types (`int`, `double`, `char*`) within a singly-linked list.

---

## Overview

Standard dynamic data structures in C are strictly typed and usually restricted to a single data type per list. This repository explores a clean pattern leveraging **C11 `_Generic` macros** combined with **tagged unions** to build a single linked list interface capable of storing multiple primitive types while providing type safety at runtime.

---

## Key Features

* **Compile-Time Macro Polymorphism (`_Generic`):** Automatically routes uniform function calls (`add_start`, `del_after`, etc.) to the appropriate type-specific implementation based on argument types.
* **Tagged Union Storage:** Each node contains an internal `enum` tag (`INT`, `STRING`, `DOUBLE`) alongside a `union` to track data identity at runtime.
* **Type-Safe Deletion:** Deletion routines verify that the extraction target matches the stored node's type tag before freeing memory, preventing garbage reads or type corruption.

---

## Data Structure Design

### Node Architecture
```c
struct Node {
    enum {
        INT, STRING, DOUBLE
    } type;
    
    union {
        double real;
        char s[100];
        int i;
    } data;

    struct Node* next;
};
```

### Macro Selection
```c
#define add_start(head, data)                            \
                        _Generic( (data),                \
                            int    : iadd_start_ll,      \
                            char*  : sadd_start_ll,      \
                            double : fadd_start_ll) ( head, data )

#define del_start(head, source)                          \
                        _Generic( (source),              \
                            int*   : idel_start_ll,      \
                            char*  : sdel_start_ll,      \
                            double*: fdel_start_ll) ( head, source )
```

---

## Usage Example

```c
#include <stdio.h>

int main(void) {
    llptr head;
    init_ll(&head);

    // Dynamic insertion across types
    add_start(&head, 10);
    add_start(&head, "Hello C11");
    add_start(&head, 3.14159);

    print_LL(head);

    // Safe extraction
    double val;
    if (del_start(&head, &val)) {
        printf("Successfully popped double: %.2f\n", val);
    } else {
        printf("Deletion failed: Type mismatch or list empty.\n");
    }

    return 0;
}
```
---
This project is intended purely for educational and experimental purposes.
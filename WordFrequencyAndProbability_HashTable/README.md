# Word Occurrence & Probability Analyzer (Hash Table)

A C program that processes input text files, indexes words using a separate-chaining hash table, tracks sentence occurrences, and computes individual and conditional word occurrence probabilities.

## Features

- **Text Normalization**: Parses punctuated sentences and strips redundant whitespace into an intermediate format (`newLines.txt`).
- **Separate-Chaining Hash Table**: Stores tokens with tail-pointer references for $O(1)$ linked-list appends during collisions.
- **Dynamic Occurrence Tracking**: Automatically resizes sentence-tracking arrays per word entry via `realloc`.
- **Statistical Analysis**:
  - Total word counts across text.
  - Marginal probability of a word appearing in any sentence: $P(A)$.
  - Conditional probability of a word appearing given another word in the same sentence: $P(A \mid B)$ (calculated via binary search over occurrence arrays).

## Intermediate Step

We want this specific format for the text we are working with:
```
length_1
sentece_1
length_2
sentece_2
length_3
sentece_3
...
```
Original 
```
I went to the store to buy some milk and bread. The weather was warm,
            so we decided to eat outside.My brother is 
watching a movie in the living room.
She usually drinks coffee before going 

to work.
I forgot my keys
 at home this
        morning. The dog was sleeping under the table.
We are planning to visit our friends this weekend.
He bought a new laptop because his old one stopped working.
There were many people waiting for the bus.


I like listening to music while I study.
```
Final
``` 
47
I went to the store to buy some milk and bread.
51
The weather was warm, so we decided to eat outside.
50
My brother is watching a movie in the living room.
47
She usually drinks coffee before going to work.
38
I forgot my keys at home this morning.
37
The dog was sleeping under the table.
50
We are planning to visit our friends this weekend.
59
He bought a new laptop because his old one stopped working.
43
There were many people waiting for the bus.
40
I like listening to music while I study.
```
---
This project is intended purely for educational and experimental purposes.
# 🌲 Dynamic Sparse Table

Implementation of a **dynamic sparse table** based on a modified **trie structure**.  
This project was created as part of my **Algorithms and Data Structures** course at *Gdańsk University of Technology*.  

---

## 📖 Description

To store values indexed by **sparse keys** from a universe *N*, common solutions like hash tables are often used.  
However, they may suffer from **poor worst-case access time**.  

An alternative is to use a **trie-based structure**, which guarantees better pessimistic time complexity.  

In this project, the trie is parameterized by two values:  
- **n** – number of children in the root  
- **k** – number of children for each next level  

## 🔑 Operations
- **Insertion** – key `x` is placed starting from the root; if taken, we move to child nodes with indices `x mod n`, then `mod k`, until a free spot is found.  
- **Search** – performed in the same way, following `mod n` / `mod k` path until the key is found or an empty node is reached.  
- **Deletion** – requires finding node `p` with key `x`. If it’s an internal node, a **leaf candidate `q`** is searched (always to the left in our strategy). Its value is copied into `p`, and node `q` is removed.  

---

## 📚References 
Tarjan, R. E. (1978). Storing a Sparse Table. Stanford University Department of Computer Science Technical Report CS-TR-78-683. Retrieved from [here](http://i.stanford.edu/pub/cstr/reports/cs/tr/78/683/CS-TR-78-683.pdf?utm_source=chatgpt.com)


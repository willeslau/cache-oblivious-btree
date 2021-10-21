# Cache Oblivious BTree

## Project Requirements
### Team Name
BBBBTree

### Give a one paragraph description of your miniproject goals and plans. 
This project aims to study and implement a cache-oblivious btree. Throughout study and work experience, btree has played 
a vital role in many applications, such as database indexing. The purpose of this project is to: 
1. Learn more about btrees by writing an implementation in C
2. Study storage and memory efficiency, have a deeper understanding of memory hierarchy model

### Paper
https://erikdemaine.org/papers/CacheObliviousBTrees_SICOMP/paper.pdf

### Give a short summary of the background material you have read so far.
The main idea of the implementation is to write sub-btrees in memory in consecutive blocks in the storage. This would reduce
the number of reads between storage and memory. At the same time, with the help of `Packed-Memory Array`, one can improve
the performance further.

### Document
Submit on Luminus:
* Summary of research paper.
* What is the main related work?
* What are the main technical ideas? (How do they solve the problem? What are the key algorithms? What are the key
theorems? What are the key proof ideas?)
* What are some interesting open problems/directions?
One of the key problems is whether their implementation is of true practical value...

### References
1. Postgres btree implementation: https://github.com/postgres/postgres/tree/master/src/backend/access/nbtree
2. Packed Memory Array implementation: https://github.com/pabmont/pma
3. Improvements on Packed Memory Array: https://www3.cs.stonybrook.edu/~bender/newpub/BenderHu07-TODS.pdf and https://ir.cwi.nl/pub/28649/28649.pdf
4. Process reference: https://www.cs.nmsu.edu/~hcao/teaching/cs582/note/DB2_4_BplusTreeExample.pdf
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

cmake_minimum_required(VERSION 3.20)
project(cache_oblivious_btree C)

set(CMAKE_C_STANDARD 99)

add_executable(
        cache_oblivious_btree
        src/btree.c
        src/btree.h
        src/hashmap.c
        src/hashmap.h
        src/util.c
        src/util.h
        src/pma.h
        src/pma.c
        src/cob.h
        src/cob.c
        tests/pma_tests.c
        tests/btree_tests.c
        tests/cobtree_tests.c
)

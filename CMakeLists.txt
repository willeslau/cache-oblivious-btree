cmake_minimum_required(VERSION 3.20)
project(cache_oblivious_btree C)

set(CMAKE_C_STANDARD 99)

add_executable(cache_oblivious_btree main.c src/btree.c src/btree.h src/hashmap.c src/hashmap.h src/serialization.c src/serialization.h src/pma.h src/pma.c src/cob.h)

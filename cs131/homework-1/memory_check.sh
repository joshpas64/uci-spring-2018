#!/bin/bash

make clean
make

valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./main 

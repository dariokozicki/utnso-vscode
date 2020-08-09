#!/bin/bash
FILE=proceso3
make $FILE
if test -f "./$FILE"; then
    valgrind --tool=memcheck --leak-check=yes --show-possibly-lost=no --show-reachable=no --num-callers=20 ./$FILE
fi
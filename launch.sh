#!/bin/sh

mkdir -p build
mkdir -p img
mkdir -p storage

rm storage/*

# gcc -O2 -march=native -g main.c nn.c -o a.out -lm
# ./a.out > data.dat
# gnuplot plot.gp


# gcc -O2 -march=native -g main.c nn.c image_processing.c -o a.out -I./lib/ -L./lib/ -lspng -lm

make -B
# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib valgrind --log-file="error.txt" ./prog.out
# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib valgrind --leak-check=full ./prog.out
# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./prog.out > data.dat
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./prog.out 

gnuplot plot.gp


 # LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./test.out 

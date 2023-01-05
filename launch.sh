#!/bin/sh


# gcc -O2 -march=native -g main.c nn.c -o a.out -lm
# ./a.out > data.dat
# gnuplot plot.gp


# gcc -O2 -march=native -g main.c nn.c image_processing.c -o a.out -I./lib/ -L./lib/ -lspng -lm

make -B
# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib valgrind --log-file="error.txt" ./prog.out
# LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib valgrind  ./prog.out
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ./prog.out > data.dat

 gnuplot plot.gp

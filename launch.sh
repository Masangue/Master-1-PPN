#!/bin/sh


gcc -O2 -march=native -g main.c nn.c -o a.out -lm
# ./a.out 
./a.out > data.dat
gnuplot plot.gp



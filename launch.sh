#!/bin/sh


gcc main.c -o a.out -lm
./a.out > data.dat
gnuplot plot.gp



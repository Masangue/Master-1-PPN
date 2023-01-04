#
set datafile separator ";"


#
set grid

#
set key autotitle columnhead
set ylabel "err"
set xlabel "id" 

set logscale y
#set logscale x

img1 = "image.png"
title1 = "error"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "data.dat" using 1:2 w p

    






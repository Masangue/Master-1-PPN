#
set datafile separator ","


#set logscale y
#set logscale x


#
set grid

#
set key autotitle columnhead
set ylabel "err"
set xlabel "epoch" 

###### PRECISION ######

img1 = "img/precision.png"
title1 = "precision"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "data.dat" using 1:2 w lp


###### RECALL ######

img1 = "img/recall.png"
title1 = "recall"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "data.dat" using 1:3 w lp


###### accuracy ######

img1 = "img/accuracy.png"
title1 = "accuracy"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "data.dat" using 1:4 w lp



###### F1 ######

img1 = "img/f1.png"
title1 = "f1"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "data.dat" using 1:5 w lp


###### ROC ######

img1 = "img/roc.png"
title1 = "ROC"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "data.dat" using 6:3 w lp











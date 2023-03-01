# this script is deprecated for now

#
set datafile separator ";"


#set logscale y
#set logscale x


#
set grid

#
set key autotitle columnhead
set ylabel "err"
set xlabel "epoch" 

###### PRECISION ######

img1 = "out/img/precision.png"
title1 = "precision"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "out/data/test.dat" using 1:2 w lp t "test",\
     "out/data/train.dat" using 1:2 w lp  t "train" 


###### RECALL ######

img1 = "out/img/recall.png"
title1 = "recall"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "out/data/test.dat" using 1:3 w lp  t "test",\
     "out/data/train.dat" using 1:3 w lp t "train" 


###### accuracy ######

img1 = "out/img/accuracy.png"
title1 = "accuracy"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "out/data/test.dat" using 1:4 w lp  t "test", \
     "out/data/train.dat" using 1:4 w lp t "train" 



###### F1 ######

img1 = "out/img/f1.png"
title1 = "f1"

set title title1

##
set terminal "png" size 800,400
set output img1

plot "out/data/test.dat" using 1:5 w lp  t "test", \
     "out/data/train.dat" using 1:5 w lp t "train" 


###### ROC ######

img1 = "out/img/roc.png"
title1 = "ROC"

set ylabel "True positive Rate"
set xlabel "False Positive Rate" 



set title title1

##
set terminal "png" size 800,400
set output img1

plot "out/data/test.dat" using 6:3 w p  t "test", \
     "out/data/train.dat" using 6:3 w p t "train" 











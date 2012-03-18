#set datafile separator ",";

set terminal postscript "Helvetica" 20
set terminal postscript color
set terminal postscript eps enhanced
set output 'sleep_model.eps'

#set xrange [0:0.28]
#set yrange [0:2.0]

#set logscale y
set xlabel "Sleep time (s)"
set ylabel "Wait time (s)"

set key left

plot "data_3n" using 1:2 every 1 title col with linespoints lt -1,\
( (0.999*0.599)/(1-0.999) + (x/2.0) ) title "model (3 nodes)" w linespoints lt -1
#"data_2n" u 1:2 every 1 title col with linespoints lt -1,\

set term tgif
set nokey

set output 'sensor29_1.obj'
set xrange [0:60000]
set yrange [-40:200]
set title 'coherence time plot'
plot 'sensorch_29.mrt2' u 1:2 w l,0.008523 w l


set output 'sensor29_2.obj'
set xrange [60000:120000]
set yrange [-40:200]
set title 'coherence time plot'
plot 'sensorch_29.mrt2' u 1:2 w l,0.008523 w l
pause -1
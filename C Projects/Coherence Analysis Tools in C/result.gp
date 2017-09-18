set term tgif
set nokey

set output 'ninomiya28_10fdi2_ch19.ascii.obj'
set yrange [-200.000000:200.000000]
set multiplot
set title 'ninomiya28_10fdi2_ch19.ascii'
set size 1,0.250000
set origin 0.0,0.000000
plot [12000.000000:14000.000000] 'ninomiya28_10fdi2_ch19.ascii' w l
set origin 0.0,0.250000
plot [14000.000000:16000.000000] 'ninomiya28_10fdi2_ch19.ascii' w l
set origin 0.0,0.500000
plot [16000.000000:18000.000000] 'ninomiya28_10fdi2_ch19.ascii' w l
set origin 0.0,0.750000
plot [18000.000000:20000.000000] 'ninomiya28_10fdi2_ch19.ascii' w l
unset multiplot
pause -1

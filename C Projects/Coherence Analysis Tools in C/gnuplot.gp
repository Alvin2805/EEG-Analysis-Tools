set term tgif
set nokey

set output 'coherence_all_mutou2014317_19_1024.obj'
set xrange [0:291.666667]
set noxzeroaxis
set noxtics
set yrange [0:0.333333]
set noyzeroaxis
set noytics
set noborder
set title
set label 1 'mutou2014317' at 0,0.316667 
set label 2 'EEG-EMG Coherence' at 0,0.291667 
set label 3 '0.050' at 66.666667,0.316667 
set label 4 '50' at 130.833333,0.260417 
set label 5 '0.025' at 66.666667,0.291667 
set label 6 '25' at 105.833333,0.260417 
set label 7 '0' at 78.333333,0.261667 
plot 'mutou2014317_newpoint' u 1:2 w l ,'mutou2014317_X_line' u 1:2 w l -1 ,'mutou2014317_Y_line' u 1:2 w l -1 ,'mutou2014317_Noise_line' u 1:2 w l 
exit 

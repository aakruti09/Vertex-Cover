set term wxt size 1080,720
set key top right box
set key width 1
set key font ",12"
set tics nomirror
set grid
set xr[0:55]
set yr[0:0.9]
set xtics 5,5,50
set ytics 0.1,0.1,0.8
set xlabel "Number of vertices"
set ylabel "Running time (miliseconds)"
set title "Approximate Vertex Cover Algorithms Running Time" font ",14"
plot 'rcv1.dat' using 1:2:3 title "Approx-1" lt 7 lc 7 w yerr, 'rcv1.dat' title '' lt 7 lc 7 w lp, 'rcv2.dat' using 1:2:3 title "Approx-2" lt 7 lc 22 w yerr, 'rcv2.dat' title '' dt 2 lc 22 w lp
set term pngcairo size 1080,720
set output "rcv.png"
replot
set term wxt size 1080,720
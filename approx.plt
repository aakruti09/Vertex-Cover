set term wxt size 1080,720
set key top left box
set key width 2
set key font ",12"
set tics nomirror
set grid
set xr[0:55]
set yr[0:40]
set xtics 0,5,55
set ytics 0,2,40
set xlabel "Number of vertices"
set ylabel "Number of vertices in Vertex Cover"
set title "Approximation Ratio" font ",14"
plot 'acnf.dat' title 'CNF-SAT' lw 2 dt 1 lt 7 lc 7 w lp,'acv1.dat' title 'Approx-1' lw 2 dt 1 lt 20 lc 20 w lp,'acv2.dat' title 'Approx-2' lw 2 dt 1 lt 9 lc 9 w lp
set term pngcairo size 1080,720
set output "approx.png"
replot
set term wxt size 1080,720
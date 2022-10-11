set term wxt size 1080,720
set key top right box
set key width 1
set key font ",12"
set tics nomirror
set grid
set xr[0:20]
set yr[0:9]
set xtics 5,5,15
set ytics 0,1,9
set xlabel "Number of vertices"
set ylabel "Running time (seconds)"
set title "CNF-SAT Running Time" font ",14"
plot 'rcnf.dat' using 1:2:3 title "CNF-SAT" lt 7 lc 9 w yerr, 'rcnf.dat' title '' dt 3 lc 9 w lp
set term pngcairo size 1080,720
set output "rcnf.png"
replot
set term wxt size 1080,720
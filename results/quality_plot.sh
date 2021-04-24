#! /bin/sh
file1=dash_festive_result.txt
file2=dash_panda_result.txt
file3=dash_tobasco_result.txt
file4=dash_osmp_result.txt
output=dash-1
gnuplot<<!
set xlabel "time/ms" 
set ylabel "quality"
set xrange [0:200000]
set yrange [0:4]
set term "png"
set output "${output}-quality.png"
plot "${file1}" u 1:3 title "festive" with steps lw 2,\
    "${file2}" u 1:3 title "panda" with steps lw 2,\
    "${file3}" u 1:3 title "tobasco" with steps lw 2,\
    "${file4}" u 1:3 title "osmp" with steps lw 2
set output
exit
!

file1=dash_raahs_result.txt
file2=dash_fdash_result.txt
file3=dash_sftm_result.txt
file4=dash_svaa_result.txt
output=dash-2
gnuplot<<!
set xlabel "time/ms" 
set ylabel "quality"
set xrange [0:200000]
set yrange [0:4]
set term "png"
set output "${output}-quality.png"
plot "${file1}" u 1:3 title "raahs" with steps lw 2,\
    "${file2}" u 1:3 title "fdash" with steps lw 2,\
    "${file3}" u 1:3 title "sftm" with steps lw 2,\
    "${file4}" u 1:3 title "svaa" with steps lw 2
set output
exit
!
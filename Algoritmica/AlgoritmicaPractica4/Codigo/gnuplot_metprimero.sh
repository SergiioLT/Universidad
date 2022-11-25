#!/bin/bash

cat << _end_ | gnuplot
set terminal postscript eps color
set output "graficoMetPrimero.eps"
set key right bottom
set xlabel "Tiempo"
set ylabel " "
plot 'metodoPrimeroSerie.txt' using 1:2 t "Serie original" w l, 'metodoPrimeroDom.txt' using 1:2 t "Serie segmentada" w l
_end_

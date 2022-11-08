#!/bin/bash

cat << _end_ | gnuplot
set terminal postscript eps color
set output "graficoMetSegundo.eps"
set key right bottom
set xlabel "Tiempo"
set ylabel " "
plot 'metodoSegundoSerie.txt' using 1:2 t "Serie original" w l, 'metodoSegundoDom.txt' using 1:2 t "Serie segmentada" w l
_end_

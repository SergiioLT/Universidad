#!/bin/bash

cat << _end_ | gnuplot
set terminal postscript eps color
set output "graficoQuickSort.eps"
set key right bottom
set xlabel "Numero de elementos"
set ylabel "Tiempo"
plot 'datosFinalesQuickSort.txt' using 1:2 t "Tiempo real de ordenacion del numero de elementos" w l, 'datosFinalesQuickSort.txt' using 1:3 t "Tiempo estimado de ordenacion del numero de elementos" w l
_end_

#!/bin/bash

cat << _end_ | gnuplot
set terminal postscript eps color
set output "graficoQuickSort.eps"
set key right bottom
set xlabel "Numero de elementos"
set ylabel "Tiempo  (microsegundos)"
plot 'datosFinalesQuickSort.txt' using 1:2 t "Tiempo reales", 'datosFinalesQuickSort.txt' using 1:3 t "Tiempo estimado" 
_end_

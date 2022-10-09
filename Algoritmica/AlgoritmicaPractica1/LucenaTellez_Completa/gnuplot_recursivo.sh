#!/bin/bash

cat << _end_ | gnuplot
set terminal postscript eps color
set output "graficoRecursivo.eps"
set key right bottom
set xlabel "Orden de matriz"
set ylabel "Tiempo (microsegundos)"
plot 'datosFinalesIterativo.txt' using 1:2 t "Tiempo reales", 'datosFinalesIterativo.txt' using 1:3 t "Tiempo estimado" w l
_end_

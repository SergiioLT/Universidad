#ifndef FUNCIONESGLOBALES_HPP
#define FUNCIONESGLOBALES_HPP

#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

bool estaOrdenado(const vector<int> &v);
void mostrarMatriz(vector < vector < double > > &v);
double sumatorio(const vector<double> &n, const vector<double> &t, int expN, int expT);
double media(vector<double> vector);
double varianza(vector<double> vector, double media);
double calcularCoeficienteDeterminacion(const vector<double> &tiemposReales, const vector<double> &tiemposEstimados);
long long int factorial(const int n);
void rellenarMatrizAleatorio(vector < vector < double > > &v);
void rellenarMatrizAMano(vector < vector < double > > &v);
void rellenarVector(vector<int> &v);

#endif
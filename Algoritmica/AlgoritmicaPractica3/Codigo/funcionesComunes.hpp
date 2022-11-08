#ifndef FUNCIONESCOMUNES_HPP
#define FUNCIONESCOMUNES_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include "serietemporal.hpp"

using namespace std;

int buscarMinMetodo(vector < long double > &error);
void eliminarPunto(vector < long double > &error, vector < int > &dominantes, int auxPosicion);
void recalcularErrores(vector < long double > &error, vector <int > &dominantes, int auxPosicion, SerieTemporal);
void recalcularErrores2(vector < long double > &error, vector <int > &dominantes, int auxPosicion, SerieTemporal, int posicionRandom);

#endif
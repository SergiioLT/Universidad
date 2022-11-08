#include "funcionesComunes.hpp"

using namespace std;

int buscarMinMetodo(vector < long double > &error){

    long double auxMin = 999999.0;
    int posicionMin;

    for(int i=0 ; i<error.size(); i++){
        if(error[i]<auxMin){
            auxMin=error[i];
            posicionMin = i;
        }
    }

    return posicionMin;
}

void eliminarPunto(vector < long double > &error, vector < int > &dominantes, int auxPosicion){

    error.erase(error.begin()+auxPosicion);
    dominantes.erase(dominantes.begin()+auxPosicion);

}

void recalcularErrores(vector < long double > &error, vector < int > &dominantes, int auxPosicion, SerieTemporal SerieMet1){

    if(auxPosicion==1){

        error[auxPosicion] = SerieMet1.calcularIseEntreDosPuntos(dominantes[auxPosicion-1],dominantes[auxPosicion+1]);

    }   else if(auxPosicion==dominantes.size()-1){

            error[auxPosicion-1] = SerieMet1.calcularIseEntreDosPuntos(dominantes[auxPosicion-2],dominantes[auxPosicion]);

        }   else {

                error[auxPosicion] = SerieMet1.calcularIseEntreDosPuntos(dominantes[auxPosicion-1],dominantes[auxPosicion+1]);

                error[auxPosicion-1] = SerieMet1.calcularIseEntreDosPuntos(dominantes[auxPosicion-2],dominantes[auxPosicion]);
            }   
}

void recalcularErrores2(vector < long double > &error, vector < int > &dominantes, int auxPosicion, SerieTemporal SerieMet1, int posicionRandom){

    if(auxPosicion==1){

        error[auxPosicion] = SerieMet1.calcularEmaxEntreDosPuntos(dominantes[auxPosicion-1],dominantes[auxPosicion+1],posicionRandom);

    }   else if(auxPosicion==dominantes.size()-1){

            error[auxPosicion-1] = SerieMet1.calcularEmaxEntreDosPuntos(dominantes[auxPosicion-2],dominantes[auxPosicion],posicionRandom);

        }   else {

                error[auxPosicion] = SerieMet1.calcularEmaxEntreDosPuntos(dominantes[auxPosicion-1],dominantes[auxPosicion+1],posicionRandom);

                error[auxPosicion-1] = SerieMet1.calcularEmaxEntreDosPuntos(dominantes[auxPosicion-2],dominantes[auxPosicion],posicionRandom);
            }   
}
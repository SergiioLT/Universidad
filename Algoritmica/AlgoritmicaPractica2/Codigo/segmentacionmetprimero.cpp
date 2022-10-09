#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <cassert>
#include "serietemporal.hpp"

using namespace std;

void errorMetodo1(int inicial, int final, double errorMaximo, SerieTemporal &serieMet1);

void segmentacionmetprimero(){

    char nombreFichero[50];
    int numeroDominantes=0, puntoErrorMaximo;
    long double ISE, errorMaximoObtenido;
    double errorMaximo=0.0;
    
    system ("clear");
    cout<<"Indique el nombre del fichero que desea obtener los puntos: "<<endl;
     cin>>nombreFichero;
    
    cout<<endl;
    cout<<"Error maximo permitido: "<< endl;
     cin>>errorMaximo;

    SerieTemporal SerieMet1(nombreFichero);

    SerieMet1.dominantePunto(0,true);
    SerieMet1.dominantePunto(SerieMet1.numeroPuntosSerieTemporal()-1,true);

    errorMetodo1(0,SerieMet1.numeroPuntosSerieTemporal()-1,errorMaximo,SerieMet1);

    SerieMet1.erroresSegmentacion(ISE,errorMaximoObtenido,puntoErrorMaximo);
    cout<<endl;
    cout<<"número de puntos dominantes: "<<SerieMet1.contarPuntosDominantes()<<endl;
    cout<<"ISE = "<<ISE<<endl;
    cout<<"errorMaximo = "<<errorMaximoObtenido<<endl;
    cout<<"puntoErrorMaximo = "<<puntoErrorMaximo<<endl<<endl;

    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);
}

void errorMetodo1(int inicial, int final, double errorMaximo, SerieTemporal &serieMet1){

    int posicionMaximo;

    double errorVertical = serieMet1.calcularEmaxEntreDosPuntos(inicial,final,posicionMaximo);

    if(errorVertical>errorMaximo){    
        serieMet1.dominantePunto(posicionMaximo,true);
        errorMetodo1(inicial,posicionMaximo,errorMaximo,serieMet1);
        errorMetodo1(posicionMaximo,final,errorMaximo,serieMet1);
    }
}
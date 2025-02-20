#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <cassert>
#include "serietemporal.hpp"
#include "recta.hpp"

using namespace std;

void errorMetodo2(int inicial, int final, double errorMaximo, SerieTemporal &serieMet2);

void segmentacionmetsegundo(){

    char nombreFichero[50];
    char ficheroGuardarSerieMet2[50] = "metodoSegundoSerie.txt";
    char ficheroGuardarDomMet2[50] = "metodoSegundoDom.txt";
    int numeroDominantes=0, puntoErrorMaximo;
    long double ISE, errorMaximoObtenido;
    double errorMaximo=0.0;
    
    system ("clear");
    cout<<"Indique el nombre del fichero que desea obtener los puntos: "<<endl;
     cin>>nombreFichero;
    
    cout<<endl;
    cout<<"Error maximo permitido: "<< endl;
     cin>>errorMaximo;

    SerieTemporal serieMet2(nombreFichero);

    serieMet2.dominantePunto(0,true);
    serieMet2.dominantePunto(serieMet2.numeroPuntosSerieTemporal()-1,true);

    errorMetodo2(0,serieMet2.numeroPuntosSerieTemporal()-1,errorMaximo,serieMet2);

    serieMet2.erroresSegmentacion(ISE,errorMaximoObtenido,puntoErrorMaximo);
    cout<<endl;
    cout<<"número de puntos dominantes: "<<serieMet2.contarPuntosDominantes()<<endl;
    cout<<"ISE = "<<ISE<<endl;
    cout<<"errorMaximo = "<<errorMaximoObtenido<<endl;
    cout<<"puntoErrorMaximo = "<<puntoErrorMaximo<<endl<<endl;

    serieMet2.guardarSerieTemporal(ficheroGuardarSerieMet2);
    serieMet2.guardarSegmentacion(ficheroGuardarDomMet2);
    system("./gnuplot_metsegundo.sh");
    //serieMet2.mostrarPuntosDominantes();

    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);
}

void errorMetodo2(int inicial, int final, double errorMaximo, SerieTemporal &serieMet2){

    int posicionMaximo;

    double errorVertical = serieMet2.calcularEmaxEntreDosPuntos(inicial,final,posicionMaximo);
    
    if(errorVertical>errorMaximo){    
        int posicionMaximo = (inicial + final) /2;
        serieMet2.dominantePunto(posicionMaximo,true);
        errorMetodo2(inicial,posicionMaximo,errorMaximo,serieMet2);
        errorMetodo2(posicionMaximo,final,errorMaximo,serieMet2);
    }
}
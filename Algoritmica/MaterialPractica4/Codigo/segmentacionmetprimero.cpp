#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <cassert>
#include <limits>
#include "serietemporal.hpp"

using namespace std;

void segmentacionmetprimero(){

    char nombreFichero[50];
    char ficheroGuardarSerieMet1[50] = "metodoPrimeroSerie.txt";
    char ficheroGuardarDomMet1[50] = "metodoPrimeroDom.txt";
    int numeroDominantes=0, puntoErrorMaximo;
    long double ISE, errorMaximoObtenido;
    int numeroPuntos=0;
    int auxPosicion;
    
    system ("clear");
    cout<<"Indique el nombre del fichero que desea obtener los puntos: "<<endl;
    cin>>nombreFichero;
    
    cout<<endl;
    cout<<"Numero de puntos finales: "<< endl;
    cin>>numeroPuntos;

    //Leer serie, crear Errores
    SerieTemporal SerieMet1(nombreFichero);
    long double E[SerieMet1.numeroPuntosSerieTemporal()][numeroPuntos-1];
    long double Father[SerieMet1.numeroPuntosSerieTemporal()][numeroPuntos-1];

    E[0][0]=0;

    for(int i=1;i<SerieMet1.numeroPuntosSerieTemporal();i++){
        E[i][0] = numeric_limits<int>::max();
    }

    //E[NumeroDePuntos-1][numeroPuntos-1];
    //e(1)(1)=e(0)(0) done
    //E[0][0]=0 done
    //E[i][0]=infinito done
    //Father[NumeroDePuntos-1][numeroPuntos-1]
    //m=1, npSegmentacion-1
    //n=m, npSeries-1
    //i=m-1,....,n-1
    




    SerieMet1.erroresSegmentacion(ISE,errorMaximoObtenido,puntoErrorMaximo);

    cout<<endl;
    cout<<"número de puntos dominantes: "<<SerieMet1.contarPuntosDominantes()<<endl;
    cout<<"ISE = "<<ISE<<endl;
    cout<<"errorMaximo = "<<errorMaximoObtenido<<endl;
    cout<<"puntoErrorMaximo = "<<puntoErrorMaximo<<endl<<endl;


    SerieMet1.guardarSerieTemporal(ficheroGuardarSerieMet1);
    SerieMet1.guardarSegmentacion(ficheroGuardarDomMet1);
    system("./gnuplot_metprimero.sh");


    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);
}


#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <bits/stdc++.h>
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
    long double auxValor;

    system ("clear");
    cout<<"Indique el nombre del fichero que desea obtener los puntos: "<<endl;
    cin>>nombreFichero;
    
    cout<<endl;
    cout<<"Numero de puntos finales: "<< endl;
    cin>>numeroPuntos;

    //Leer serie, crear Errores y rellenarlos
    SerieTemporal SerieMet1(nombreFichero);
    long double E[SerieMet1.numeroPuntosSerieTemporal()][numeroPuntos];
    int Father[SerieMet1.numeroPuntosSerieTemporal()][numeroPuntos];

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

    for(int m=1; m<numeroPuntos; m++){
        for(int n=m; n<SerieMet1.numeroPuntosSerieTemporal(); n++){
            //Inicializo para buscar el menor de las siguientes combinaciones
            auxValor = numeric_limits<long double>::max();

                //Se realizan todas las combinaciones
                for(int j=m-1; j<n; j++){
                    //Se guarda el menor y se reserva la posicion del menor para guardarlo como Father
                    if((E[j][m-1] + SerieMet1.calcularIseEntreDosPuntos(j,n))<auxValor){
                        auxValor = (E[j][m-1] + SerieMet1.calcularIseEntreDosPuntos(j,n));
                        E[n][m] = auxValor;
                        Father[n][m] = j;
                    }
                }
        }
    }

    SerieMet1.dominantePunto(0,true);
    SerieMet1.dominantePunto(SerieMet1.numeroPuntosSerieTemporal()-1,true);

    int Fila = SerieMet1.numeroPuntosSerieTemporal()-1;
    int Columna = numeroPuntos-1;
    vector<int> auxPosiciones;
    auxPosiciones.push_back(SerieMet1.numeroPuntosSerieTemporal()-1);
    
    while(SerieMet1.contarPuntosDominantes()<numeroPuntos){

        SerieMet1.dominantePunto(Father[Fila][Columna],true);  
        auxPosiciones.push_back(Father[Fila][Columna]);

        Fila = Father[Fila][Columna];
        Columna--;
    }

    sort(auxPosiciones.begin(),auxPosiciones.end());
    SerieMet1.erroresSegmentacion(ISE,errorMaximoObtenido,puntoErrorMaximo);

    cout<<endl;
    //cout<<"número de puntos dominantes: "<<SerieMet1.contarPuntosDominantes()<<endl;
    cout<<"ISE = "<<ISE<<endl;
    cout<<"errorMaximo = "<<errorMaximoObtenido<<endl;
    cout<<"puntoErrorMaximo = "<<puntoErrorMaximo<<endl<<endl;
    cout<<"puntos óptimos de segmentación: ";
    for(int i=0;i<auxPosiciones.size();i++){
        cout<<auxPosiciones[i];
    }
    cout<<endl;

    SerieMet1.guardarSerieTemporal(ficheroGuardarSerieMet1);
    SerieMet1.guardarSegmentacion(ficheroGuardarDomMet1);
    system("./gnuplot_metprimero.sh");


    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);
}


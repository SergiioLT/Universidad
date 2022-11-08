#include <vector>    // necesario para el contenedor Vector2D
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <cassert>
#include "serietemporal.hpp"
#include "funcionesComunes.hpp"

using namespace std;

void segmentacionmetsegundo(){

    char nombreFichero[50];
    char ficheroGuardarSerieMet1[50] = "metodoSegundoSerie.txt";
    char ficheroGuardarDomMet1[50] = "metodoSegundoDom.txt";
    int puntoErrorMaximo;
    long double ISE, errorMaximoObtenido;
    int numeroPuntos=0;
    int posicionRandom;


    int auxPosicion;
    
    system ("clear");
    cout<<"Indique el nombre del fichero que desea obtener los puntos: "<<endl;
    cin>>nombreFichero;
    
    cout<<endl;
    cout<<"Numero de puntos finales: "<< endl;
    cin>>numeroPuntos;

    SerieTemporal SerieMet1(nombreFichero);
    vector < long double > error(SerieMet1.numeroPuntosSerieTemporal());
    vector < int > dominantes(SerieMet1.numeroPuntosSerieTemporal());

    //Hacer dos vectores uno tendra los puntos dominantes y otro tendra sus respectivos ISE y eMax
    //Hacer todos dominantes
    for(int i=0; i<SerieMet1.numeroPuntosSerieTemporal(); i++){
        dominantes[i]=i;
    }

    //Para calcular ISE es error(i-1,i+1)
    error[0] = 9999999999;
    error[SerieMet1.numeroPuntosSerieTemporal()-1] = 999999999;

    for(int i=1; i<SerieMet1.numeroPuntosSerieTemporal()-1; i++){
        error[i] = SerieMet1.calcularEmaxEntreDosPuntos(i-1,i+1,posicionRandom);
    }

    //Calcular punto de error minimo
    //eliminar dominante[x] y error[x]
    //recalcuar error(dominante[x-1] y calcular el dominante[x])
    while(dominantes.size()>numeroPuntos){
        auxPosicion = buscarMinMetodo(error);
        eliminarPunto(error,dominantes,auxPosicion);
        recalcularErrores2(error,dominantes,auxPosicion, SerieMet1, posicionRandom);
    }

    for(int i=0;i<dominantes.size();i++){
        SerieMet1.dominantePunto(dominantes[i],true);
    }
    SerieMet1.erroresSegmentacion(ISE,errorMaximoObtenido,puntoErrorMaximo);

    cout<<endl;
    cout<<"número de puntos dominantes: "<<SerieMet1.contarPuntosDominantes()<<endl;
    cout<<"ISE = "<<ISE<<endl;
    cout<<"errorMaximo = "<<errorMaximoObtenido<<endl;
    cout<<"puntoErrorMaximo = "<<puntoErrorMaximo<<endl<<endl;


    SerieMet1.guardarSerieTemporal(ficheroGuardarSerieMet1);
    SerieMet1.guardarSegmentacion(ficheroGuardarDomMet1);
    system("./gnuplot_metsegundo.sh");


    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);
}


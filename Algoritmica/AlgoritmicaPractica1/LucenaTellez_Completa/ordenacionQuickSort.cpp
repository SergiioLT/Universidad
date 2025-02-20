#include <iostream>
#include <vector> 
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <bits/stdc++.h>
#include <fstream>

#include "sistemaEcuaciones.hpp"
#include "funcionesGlobales.hpp"
#include "ClaseTiempo.cpp"

using namespace std;

//////////////////////////////////
// DEFINICIONES DE FUNCIONES    //
//////////////////////////////////

void tiemposOrdenacionQuickSort(int nMin, int nMax, int repeticiones,int incremento, vector<double> &tiemposReales, vector<double> &numeroElementos);
void ajusteNlogN(const vector <double> &numeroElementos, const vector <double> &tiemposReales, vector <double> &a);
void calcularTiemposEstimadosNlogN(const vector<double> &numeroElementos, const vector<double> &tiemposReales,const vector<double> &a, vector<double> &tiemposEstimados);
void calcularTiempoEstimadoNlogN(const double &n, vector<double> &a);

//////////////////////////////////
// FUNCION PRINCIPAL            //
//////////////////////////////////

void ordenacionQuickSort(){

    srand(time(NULL));

    int minimo,maximo,incremento,repeticiones;
    double cof;

    cout<<"Introduzca valor minimo del numero de elementos del vector"<<endl;
    cin>>minimo;

    cout<<"Introduzca valor maximo del numero de elementos del vector"<<endl;
    cin>>maximo;

    cout<<"Introduzca el incremento del valor del numero de elementos"<<endl;  
    cin>>incremento;

    cout<<"Introduzca el numero de repeticiones"<<endl;  
    cin>>repeticiones;

    vector<double> tiemposR;
    vector<double> Vn;
    tiemposR.clear();
    Vn.clear();
    
    tiemposOrdenacionQuickSort(minimo,maximo,repeticiones,incremento,tiemposR,Vn);

    vector<double> a;
    a.resize(2);
    a.clear();

    ajusteNlogN(Vn,tiemposR,a);

    vector<double> tiemposE;
    tiemposE.clear();

    calcularTiemposEstimadosNlogN(Vn,tiemposR,a,tiemposE);
 
    cof = calcularCoeficienteDeterminacion(tiemposR,tiemposE);

        ofstream f1("datosFinalesQuickSort.txt");
        for(int i=0; i<Vn.size();i++){
            f1<<Vn[i]<< " " <<tiemposR[i]<< " " <<tiemposE[i]<<endl;
        }
    f1.close();

    cout<<endl<<"El coeficiente de determinacion del ajuste es: "<<cof<<endl;

    system("./gnuplot_quicksort.sh");

    double n;
    double dias;

    cout<<"Desea hacer una estimacion de tiempos para un determinado tamaño"<<endl;
    cout<<"0.- No"<<endl;
    cout<<"1.- Si"<<endl;
    cin>>n;

    while(n!=0){
        cout<<"Introduzca el tamaño del ejemplar que desea (0 para salir)"<<endl<<endl;
        cout<<"Tamaño elegido: ";
        cin>>n;

        if(n!=0){
            calcularTiempoEstimadoNlogN(n,a);
            //cout<<"Para un vector de "<<n<<" elementos, se tarda en ordenarlo "<< dias<<" dias"<<endl;
        }
    }
    
}


//////////////////////////////////
// CODIGO DE LAS FUNCIONES      //
//////////////////////////////////

void tiemposOrdenacionQuickSort(int nMin, int nMax, int repeticiones,int incremento, vector<double> &tiemposReales, vector<double> &numeroElementos){
    
    Clock time;

    double media=0;
    vector<int> v;
    v.clear();

    ofstream f("tiempoRealesQS.txt");  //Fichero donde se guardaran los datos

    for(int i=nMin; i<=nMax; i=i+incremento){ //Recorrer el numero de incremento y las repeticiones por cada incremento
        v.resize(i);

        for(int j=0; j<repeticiones;j++){
            rellenarVector(v);

            time.start();
            sort(v.begin(),v.end());
            time.stop();

            media=media+time.elapsed();
            v.clear();
        }

        media=media/repeticiones; //Media de las repeticiones
        
        tiemposReales.push_back(media); //Guardar media

        numeroElementos.push_back(i);  //Guardar tamaño realizado

        f<<i<< " " <<media<<endl;

        cout<<"Para el numero de elementos "<<i<<" se ha tardado "<< media<<endl;

        media=0;
    }
    f.close();
}

void ajusteNlogN(const vector <double> &numeroElementos, const vector <double> &tiemposReales, vector <double> &a){
	
    vector <vector <double> > A(2,vector<double>(2));
	vector <vector <double> > B(2,vector<double>(1));
	vector <vector <double> > X(2,vector<double>(1));

    std::vector<double> auxiliar;
    for (size_t i = 0; i < numeroElementos.size(); i++)
	{
		auxiliar.push_back(numeroElementos[i]*log(numeroElementos[i]));
	}

    //A es la matriz con sumatorios
    A[0][0]=auxiliar.size();
    A[0][1]=sumatorio(auxiliar,auxiliar,1,0);
    A[1][0]=sumatorio(auxiliar,auxiliar,1,0);
    A[1][1]=sumatorio(auxiliar,auxiliar,2,0);
  
    //B es la matriz de terminos independientes
    B[0][0]=sumatorio(auxiliar,tiemposReales,0,1);
    B[1][0]=sumatorio(auxiliar,tiemposReales,1,1);

	resolverSistemaEcuaciones(A, B, 2, X); //X es la matriz solucion
  
	a[0]=X[0][0];
    a[1]=X[1][0];

    cout<<endl;
    cout<<"Curva:t(n)="<<a[0]<<"+"<<a[1]<<"*n*log(n)"<<endl;
    cout<<endl;
}

void calcularTiemposEstimadosNlogN(const vector<double> &numeroElementos, const vector<double> &tiemposReales,const vector<double> &a, vector<double> &tiemposEstimados){
    for(size_t i=0; i<numeroElementos.size();i++){
        tiemposEstimados.push_back(a[0] + (a[1]*numeroElementos[i]*log(numeroElementos[i])));
    }
}

void calcularTiempoEstimadoNlogN(const double &n, vector<double> &a){
    long int microsegundos= a[0]+a[1]*n*log(n);

    int segundos = (microsegundos/1000000LL)%60LL;
    int minutos = (microsegundos/(1000000LL*60LL))%60LL;
    int horas = (microsegundos/(1000000LL*60LL*60LL))%24LL;
    int dias = (microsegundos/(1000000LL*60LL*60LL*24LL))%365LL;
    int anos = microsegundos/ (1000000LL*60LL*60LL*24LL*365LL);

    cout<<"Se ha tardado en estimar--> "<<anos<< " años "<<dias<<" dias "<<horas<<" horas "<< minutos<<" minutos "<<segundos<<" segundos "<<endl;
}

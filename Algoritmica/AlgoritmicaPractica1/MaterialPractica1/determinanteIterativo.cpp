
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <fstream>

#include "sistemaEcuaciones.cpp"
#include "ClaseTiempo.cpp"
#include "funcionesGlobales.cpp"


using namespace std;

//////////////////////////////////
// DEFINICIONES DE FUNCIONES    //
//////////////////////////////////

void tiemposDeterminanteIterativo(int nMin, int nMax,int incremento, vector<double> &tiemposReales, vector<double> &numeroElementos);
void ajustePolinomico(const vector <double> &numeroElementos, const vector <double> &tiemposReales, vector <double> &a);
void calcularTiemposEstimadosPolinomico(const vector<double> &numeroElementos, const vector<double> &tiemposReales,const vector<double> &a, vector<double> &tiemposEstimados);
double calcularTiempoEstimadoPolinomico(const double &n, vector<double> &a);

//////////////////////////////////
// FUNCION PRINCIPAL            //
//////////////////////////////////
void productoMatricesCuadradas(){
    srand(time(NULL));

    int minimo,maximo,incremento;
    double cof;

    cout<<"Introduzca valor minimo del numero de elementos del vector"<<endl;
    cin>>minimo;

    cout<<"Introduzca valor maximo del numero de elementos del vector"<<endl;
    cin>>maximo;

    cout<<"Introduzca el incremento del valor del numero de elementos"<<endl;  
    cin>>incremento;

    vector<double> tiemposR;
    vector<double> Vn;
    tiemposR.clear();
    Vn.clear();
    
    tiemposDeterminanteIterativo(minimo,maximo,incremento,tiemposR,Vn);

    vector<double> a;
    a.resize(4);
    a.clear();

    ajustePolinomico(Vn,tiemposR,a);

    vector<double> tiemposE;
    tiemposE.clear();

    calcularTiemposEstimadosPolinomico(Vn,tiemposR,a,tiemposE);
 
    cof = calcularCoeficienteDeterminacion(tiemposR,tiemposE);

        ofstream f1("datosFinalesIterativo.txt");
        for(int i=0; i<Vn.size();i++){
            f1<<Vn[i]<< " " <<tiemposR[i]<< " " <<tiemposE[i]<<endl;
        }
    f1.close();

    cout<<endl<<"El coeficiente de determinacion del ajuste es: "<<cof<<endl;

    //Se lanza el script para realizar la grafica
    system("./gnuplot_iterativo.sh");

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
            dias=calcularTiempoEstimadoPolinomico(n,a);
            cout<<"Para un vector de "<<n<<" elementos, se tarda en ordenarlo "<< dias<<" dias"<<endl;
        }
    }
    
}

//////////////////////////////////
// CODIGO DE LAS FUNCIONES      //
//////////////////////////////////
void tiemposDeterminanteIterativo(int nMin, int nMax,int incremento, vector<double> &tiemposReales, vector<double> &numeroElementos){
    
    vector < vector < double > > matriz1;
    vector < vector < double > > matriz2;
    vector < vector < double > > resultado;

    Clock time;

    double tiempo_aux;

    ofstream f("TiemposRealesIterativo.txt");

    for(int i =nMin; i<=nMax;i=i+incremento){
        matriz1 = vector < vector < double > > (i,vector<double>(i));  //Matriz de tamaño IxI
        matriz2 = vector < vector < double > > (i,vector<double>(i));   
        resultado = vector < vector < double > > (i,vector<double>(i)); //Matriz donde se almacenara el resultado

        rellenarMatrizAleatorio(matriz1);  //Se rellenan automaticamente ambas matrices
        rellenarMatrizAleatorio(matriz2);

        time.start();


        time.stop();

        
        tiempo_aux=time.elapsed();
        tiemposReales.push_back(tiempo_aux);
        numeroElementos.push_back(i);

        f<<i<< " " <<tiempo_aux<<endl;

        cout<<"Para el numero de elementos "<<i<<" se ha tardado "<< tiempo_aux<<endl;

    }

    f.close();

}

void ajustePolinomico(const vector <double> &numeroElementos, const vector <double> &tiemposReales, vector <double> &a){
	vector <vector <double> > A(4,vector<double>(4));
	vector <vector <double> > B(4,vector<double>(1));
	vector <vector <double> > X(4,vector<double>(1));

    /*
    for(int i=0;i<;i++){
        for(int j=0;j<;j++){
            A[][]=sumatorio(numeroElementos,numeroElementos,,)
        }
    }

    for(int i=0;i<;i++){
        for(int j=0;j<;j++){
            B[][]=sumatorio(numeroElementos,tiemposReales,,)
        }
    }
    */

    //A es la matriz con sumatorios
    A[0][0]=numeroElementos.size();
    A[0][1]=sumatorio(numeroElementos,numeroElementos,1,0);
    A[0][2]=sumatorio(numeroElementos,numeroElementos,2,0);
    A[0][3]=sumatorio(numeroElementos,numeroElementos,3,0);


    A[1][0]=sumatorio(numeroElementos,numeroElementos,1,0);
    A[1][1]=sumatorio(numeroElementos,numeroElementos,2,0);
    A[1][2]=sumatorio(numeroElementos,numeroElementos,3,0);
    A[1][3]=sumatorio(numeroElementos,numeroElementos,4,0);

    A[2][0]=sumatorio(numeroElementos,numeroElementos,2,0);
    A[2][1]=sumatorio(numeroElementos,numeroElementos,3,0);
    A[2][2]=sumatorio(numeroElementos,numeroElementos,4,0);
    A[2][3]=sumatorio(numeroElementos,numeroElementos,5,0);

    A[3][0]=sumatorio(numeroElementos,numeroElementos,3,0);
    A[3][1]=sumatorio(numeroElementos,numeroElementos,4,0);
    A[3][2]=sumatorio(numeroElementos,numeroElementos,5,0);
    A[3][3]=sumatorio(numeroElementos,numeroElementos,6,0);

    //B es la matriz de terminos independientes
    B[0][0]=sumatorio(numeroElementos,tiemposReales,0,1);
    B[1][0]=sumatorio(numeroElementos,tiemposReales,1,1);
    B[2][0]=sumatorio(numeroElementos,tiemposReales,2,1);
    B[3][0]=sumatorio(numeroElementos,tiemposReales,3,1);
   
	resolverSistemaEcuaciones(A, B, 4, X); //X es la matriz solucion
  
	a[0]=X[0][0];
    a[1]=X[1][0];
    a[2]=X[2][0];
    a[3]=X[3][0];

    cout<<endl;
    cout<<"Curva:t(n)="<<a[0]<<"+"<<a[1]<<"*n+"<<a[2]<<"*n^2+"<<a[3]<<"*n^3"<<endl;
    cout<<endl;
}

void calcularTiemposEstimadosPolinomico(const vector<double> &numeroElementos, const vector<double> &tiemposReales,const vector<double> &a, vector<double> &tiemposEstimados){
    for(size_t i=0; i<numeroElementos.size();i++){
        tiemposEstimados.push_back(a[0] + (a[1]*numeroElementos[i]) + (a[2]*numeroElementos[i]*numeroElementos[i]) + (a[3]*numeroElementos[i]*numeroElementos[i]*numeroElementos[i]) );
    }
}

double calcularTiempoEstimadoPolinomico(const double &n, vector<double> &a){
    double dias= a[0]+a[1]*n+a[2]*n*n+a[3]*n*n*n;
    dias=dias*(1.1574*pow(10,-11)); //1 microsegundo es 1.1574e-11 dias
      
    return dias;
}

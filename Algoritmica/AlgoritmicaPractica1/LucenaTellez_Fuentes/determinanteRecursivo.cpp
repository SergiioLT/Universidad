
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <fstream>

#include "sistemaEcuaciones.hpp"
#include "funcionesGlobales.hpp"
#include "ClaseTiempo.cpp"

using namespace std;

//////////////////////////////////
// DEFINICIONES DE FUNCIONES    //
//////////////////////////////////

void tiemposDeterminanteRecursivo(int nMin, int nMax,int incremento, vector<double> &tiemposReales, vector<double> &numeroElementos);
void ajusteFactorial(const vector <double> &numeroElementos, const vector <double> &tiemposReales, vector <double> &a);
void calcularTiemposEstimadosFactorial(const vector<double> &numeroElementos, const vector<double> &tiemposReales,const vector<double> &a, vector<double> &tiemposEstimados);
void calcularTiempoEstimadoFactorial(const double &n, vector<double> &a);

//////////////////////////////////
// FUNCION PRINCIPAL            //
//////////////////////////////////
void determinanteRecursivo(){
    srand(time(NULL));

    int minimo,maximo,incremento;
    double cof;

    cout<<"Introduzca el orden minimo de la matriz"<<endl;
    cin>>minimo;

    cout<<"Introduzca el orden maximo de la matriz"<<endl;
    cin>>maximo;

    cout<<"Introduzca el aumento del orden de la matriz"<<endl;  
    cin>>incremento;

    vector<double> tiemposR;
    vector<double> Vn;
    tiemposR.clear();
    Vn.clear();
    
    tiemposDeterminanteRecursivo(minimo,maximo,incremento,tiemposR,Vn);

    vector<double> a;
    a.resize(4);
    a.clear();

    ajusteFactorial(Vn,tiemposR,a);

    vector<double> tiemposE;
    tiemposE.clear();

    calcularTiemposEstimadosFactorial(Vn,tiemposR,a,tiemposE);
 
    cof = calcularCoeficienteDeterminacion(tiemposR,tiemposE);

        ofstream f1("datosFinalesRecursivo.txt");
        for(int i=0; i<Vn.size();i++){
            f1<<Vn[i]<< " " <<tiemposR[i]<< " " <<tiemposE[i]<<endl;
        }
    f1.close();

    cout<<endl<<"El coeficiente de determinacion del ajuste es: "<<cof<<endl;

    //Se lanza el script para realizar la grafica
    system("./gnuplot_recursivo.sh");

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
            calcularTiempoEstimadoFactorial(n,a);
            //cout<<"Para un vector de "<<n<<" elementos, se tarda en ordenarlo "<< dias<<" dias"<<endl;
        }
    }
    
}

//////////////////////////////////
// CODIGO DE LAS FUNCIONES      //
//////////////////////////////////

void getCofactor(const vector<vector<double> >&mat, vector<vector<double>> &temp, int p, int q, int n)
{
	int i = 0, j = 0;

	// Looping for each element of the matrix
	for (int row = 0; row < n; row++)
	{
		for (int col = 0; col < n; col++)
		{
			// Copying into temporary matrix only those
			// element which are not in given row and
			// column
			if (row != p && col != q)
			{
				temp[i][j++] = mat[row][col];

				// Row is filled, so increase row index and
				// reset col index
				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

int determinantOfMatrix(const vector < vector<double> > &mat, int n)
{
	int D = 0; // Initialize result

	// Base case : if matrix contains single element
	if (n == 1)
		return mat[0][0];

	vector <vector <double> > temp(n,vector<double>(n)); // To store cofactors

	int sign = 1; // To store sign multiplier

	// Iterate for each element of first row
	for (int f = 0; f < n; f++)
	{
		// Getting Cofactor of mat[0][f]
		getCofactor(mat, temp, 0, f, n);
		D += sign * mat[0][f]
			* determinantOfMatrix(temp, n - 1);

		// terms are to be added with alternate sign
		sign = -sign;
	}

	return D;
}

void tiemposDeterminanteRecursivo(int nMin, int nMax,int incremento, vector<double> &tiemposReales, vector<double> &numeroElementos){
    
    int opcion;
    cout<<"Introduzca si quieres rellenar la matriz manualmente o automaticamente"<<endl;
    cout<<"1--> Manualmente"<<endl;
    cout<<"2--> Automaticamente"<<endl;
    cin>>opcion;

    vector < vector < double > > matriz1;
    vector < vector < double > > resultado;

    Clock time;

    double tiempo_aux;

    ofstream f("TiemposRealesRecursivo.txt");

    for(int i =nMin; i<=nMax;i=i+incremento){
        matriz1 = vector < vector < double > > (i,vector<double>(i));  //Matriz de tamaño IxI

        if(opcion==2){
            rellenarMatrizAleatorio(matriz1);  //Se rellenan automaticamente ambas matrices
        }else{
            rellenarMatrizAMano(matriz1);
        }

        time.start();

        determinantOfMatrix(matriz1, i);

        time.stop();

        
        tiempo_aux=time.elapsed();
        tiemposReales.push_back(tiempo_aux);
        numeroElementos.push_back(i);

        f<<i<< " " <<tiempo_aux<<endl;

        cout<<"Para el orden de la matriz "<<i<<" se ha tardado "<< tiempo_aux<<endl;

    }

    f.close();

}

void ajusteFactorial(const vector <double> &numeroElementos, const vector <double> &tiemposReales, vector <double> &a){
	vector <vector <double> > A(2,vector<double>(2));
	vector <vector <double> > B(2,vector<double>(1));
	vector <vector <double> > X(2,vector<double>(1));

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
    std::vector<double> auxiliar;
    for (size_t i = 0; i < numeroElementos.size(); i++)
	{
		auxiliar.push_back(factorial(numeroElementos[i]));
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
    cout<<"Curva:t(n)="<<a[0]<<"+"<<a[1]<<"*n!"<<endl;
    cout<<endl;
}

void calcularTiemposEstimadosFactorial(const vector<double> &numeroElementos, const vector<double> &tiemposReales,const vector<double> &a, vector<double> &tiemposEstimados){
    for(size_t i=0; i<numeroElementos.size();i++){
        tiemposEstimados.push_back(a[0] + (a[1]*factorial(numeroElementos[i])));
    }
}

void calcularTiempoEstimadoFactorial(const double &n, vector<double> &a){
    long int microsegundos= a[0]+a[1]*factorial(n);

    long int segundos = (microsegundos/1000000LL)%60LL;
    long int minutos = (microsegundos/(1000000LL*60LL))%60LL;
    long int horas = (microsegundos/(1000000LL*60LL*60LL))%24LL;
    long int dias = (microsegundos/(1000000LL*60LL*60LL*24LL))%365LL;
    long int anos = microsegundos/ (1000000LL*60LL*60LL*24LL*365LL);

    cout<<"Se ha tardado en estimar--> "<<anos<< " años "<<dias<<" dias "<<horas<<" horas "<< minutos<<" minutos "<<segundos<<" segundos "<<endl;
}

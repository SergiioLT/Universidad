#include "funcionesGlobales.hpp"

using namespace std;

bool estaOrdenado(const vector<int> &v){
    for(size_t i=0; i<v.size();i++){
        if(v[i]>v[i+1]){
            return false;
        }
    }
    return true;
}

void mostrarMatriz(vector < vector < double > > &v){  
	for(unsigned int i = 0; i < v.size(); i++){ //Recorre filas 
	
		for(unsigned int j = 0; j < v[0].size(); j++){ //Recorre columnas 
		
			cout<<v[i][j]<<" ";
		}
		cout<<endl;
	}

	cout<<endl;
}

double sumatorio(const vector<double> &n, const vector<double> &t, int expN, int expT){
	double sumatorio=0;

	for(size_t i=0; i<n.size();i++){
		sumatorio += (pow(n[i],expN)*pow(t[i],expT));
	}

	return sumatorio;
}

double media(vector<double> vector){
	double suma=0.0;
	for(size_t i=0;i<vector.size();i++){
		suma+=vector[i];
	}

	suma=suma/vector.size();
	return suma;
}

double varianza(vector<double> vector, double media){
	double var=0.0;
	for(size_t i=0;i<vector.size();i++){
		var+=((vector[i]-media)*(vector[i]-media));
	}

	var=var/vector.size();
	return var;
}

double calcularCoeficienteDeterminacion(const vector<double> &tiemposReales, const vector<double> &tiemposEstimados){
    double varianza_real=0.0;
    double varianza_estimada=0.0;
    double media_real=0.0;
    double media_estimada=0.0;

    media_real=media(tiemposReales);
    media_estimada=media(tiemposEstimados);

    varianza_real=varianza(tiemposReales,media_real);
    varianza_estimada=varianza(tiemposEstimados,media_estimada);

    return varianza_estimada/varianza_real;

}

long long int factorial(const int n){
    long long int factorial = 1;

    if(n<0){
        cout<<"No se puede hacer factorial de un numero negativo"<<endl;
    }else{
        for(int i=1;i<=n;i++){
            factorial *= i;
        }
    }
    return factorial;
}

void rellenarMatrizAleatorio(vector < vector < double > > &v){
	for(unsigned int i = 0; i < v.size(); i++){ 
		for(unsigned int j = 0; j < v[0].size(); j++){ 
			v[i][j] = drand48() * (1.05-0.95) + 0.95;   //drand48 para obtener numeros decimales aleatorios
		}
	}
}

void rellenarMatrizAMano(vector < vector < double > > &v){
	double auxiliar;
	for(unsigned int i = 0; i < v.size(); i++){ 
		for(unsigned int j = 0; j < v[0].size(); j++){ 
			cout<<"Introduzca el valor de la posicion v["<<i<<"]["<<j<<"]"<<endl;
			cin>>auxiliar;
			v[i][j] = auxiliar;  
		}
	}
}


void rellenarVector(vector<int> &v){
    for(size_t i=0;i<v.size();i++){
        v[i] = rand()%10000000;
    }
}
#include <iostream>
#include <vector>
#include "ClaseTiempo.cpp"

using namespace std;

//////////////////////////////////
// DEFINICIONES DE FUNCIONES    //
//////////////////////////////////

void imprimir(vector < vector < int > > Soluciones);
void imprimir(vector< int > Solucion);
bool Lugar(int k, vector<int> Reinas);

//////////////////////////////////
//     FUNCIONES GLOBALES       //
//////////////////////////////////

bool Lugar(int k, vector<int> Reinas){
    for(int i=0; i<k; i++){
        if( (Reinas[i]==Reinas[k]) || (abs(Reinas[i]- Reinas[k])==abs(i-k))){
            return false;
            }
        if( (Reinas[i]==Reinas[k]-2 && i == k-1 ) || (Reinas[i]==Reinas[k]-2 && i == k+1) || (Reinas[i]==Reinas[k]+2 && i == k-1 ) || (Reinas[i]==Reinas[k]+2 && i == k+1) ||
            (Reinas[i]==Reinas[k]-1 && i == k-2 ) || (Reinas[i]==Reinas[k]-1 && i == k+2) || (Reinas[i]==Reinas[k]+1 && i == k-2 ) || (Reinas[i]==Reinas[k]-1 && i == k+2)){
            return false;
            }
    }
    return true;
}

void imprimir(vector < vector <int > > Soluciones){
    for(int i=0; i<Soluciones.size();i++){
        cout<<"Solucion "<< i+1<<" --> ";
        for(int j=0;j<Soluciones[i].size();j++){
            cout<<" "<<Soluciones[i][j];
        }
        cout<<endl;
    }
}

void imprimir(vector<int> Solucion){
    cout<<endl;
    cout<<"Solucion --> ";
    for(int i=0;i<Solucion.size();i++){
        cout<<" "<<Solucion[i];
    }
    cout<<endl;
}

//////////////////////////////////
//    N REINAS BACKTRACKING     //
//////////////////////////////////


void nReinasBacktracking(int x){

    int n;
    cout<<"Introduce el tamaño del tablero (minimo 10 para obtener solucion)"<<endl;
    cin>>n;

    int k=0;
    vector<int> Reinas;
    vector< vector< int> > Soluciones;
    long double tiempo;
    Clock time;

    Reinas.resize(n);
    Reinas[0] = 0;

    time.start();

    while(k>=0){

        Reinas[k] = Reinas[k]+1;

        while(Reinas[k]<=n && Lugar(k,Reinas)==false){
            Reinas[k]=Reinas[k]+1;
        }

        if(Reinas[k]<=n){

            if(k==n-1){
                Soluciones.push_back(Reinas);
                if(x==2){
                    break;
                }
            }else{
                k = k+1;
                Reinas[k]=0;
            }

        }else{
            k=k-1;
        }
    }
    time.stop();

    cout<<endl;
    imprimir(Soluciones);
    tiempo = time.elapsed()/1000;
    cout<<"Tiempo para encontrar solucion: "<<tiempo<<" milisegundos"<<endl;
    cout<<endl;
    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);
}

//////////////////////////////////
//  N REINAS MEDIANTE VEGAS     //
//////////////////////////////////

bool LasVegas(int n, vector<int> &solucion){

    vector < int > ok(n);
    int contador = 0;
    int columna = 0;

    for(int k = 0; k < n; k++){ 
        contador = 0;

        for(int j = 0; j < n; j++){ 
            solucion[k] = j + 1; 

            if((Lugar(k,solucion) == true)){ 
               if(contador+1<n){
                contador++;
                ok[contador] = j + 1;
               }
            }
        }

        if(contador == 0){ 
            break;
        }
        
        columna = ok[rand()%contador+1];
        solucion[k] = columna;
    }

    if(contador == 0){
        return false;
    }else{
        return true;
    }
}


int repetirLasVegas(int n, vector < int > &solucion){
    bool exito;
    int intentos =0;

    do{
        intentos++;
        exito=LasVegas(n,solucion);
    }while(exito==false);

    return intentos;

}


void nReinasVegas(){

    int n = 0;
    cout<<"Introduce el tamaño del tablero (minimo 10 para obtener solucion)"<<endl;
    cin>>n;

    int intentos = 0; 
    long double tiempo; 
    Clock time; 

    vector < int > solucion(n,0);

    time.start();

    intentos = repetirLasVegas(n,solucion);

    time.stop();

    imprimir(solucion);
    tiempo = time.elapsed()/1000;
    cout<<"Tiempo para encontrar solucion: "<<time.elapsed()/1000<<" milisegundos"<<endl;
    cout<<"Numero de intentos realizados: "<<intentos<<endl;

    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);

}
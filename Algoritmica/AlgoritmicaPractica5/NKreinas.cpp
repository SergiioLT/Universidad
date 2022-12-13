#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

void imprimir(vector < vector < int > > Soluciones);
void imprimir(vector< int > Solucion);
bool Lugar(int k, vector<int> Reinas);
bool LasVegas(int n, vector<int> &Solucion);

void NKreinasBacktracking(int x){

    int n;

    cout<<"Introduce el tamaño de las n reinas"<<endl;
    cin>>n;

    int k=0;
    vector<int> Reinas;
    vector< vector< int> > Soluciones;

    Reinas.resize(n);
    Reinas[0] = 0;

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

    imprimir(Soluciones);

    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);
}

void NKreinasVegas(){
    srand(time(NULL));
    int n;
    cout<<"Introduce el tamaño de las n reinas"<<endl;
    cin>>n;

    vector<int> Solucion;
    Solucion.resize(n);
    bool exito;
    int intentos =0;

    do{
        intentos++;
        exito=LasVegas(n,Solucion);
    }while(exito==false);

    imprimir(Solucion);
    cout<<"Se han hecho "<<intentos<<" intentos"<<endl;
    do{ 
        cin.ignore();
        cout<<"Presiona intro para continuar";\
        getchar();
    } while(0);

}

bool LasVegas(int n, vector<int> &Solucion){
    
    // int columna=0;
	// int contador=0;
	// vector<int> ok;
    // ok.resize(n);

	// for(int i=0;i<n;i++){
	// 	Solucion[i]=0;
	// }

	// for(int k=0;k<n;k++){
	// 	contador=0;

	// 	for(int j=0;j<n;j++){
	// 		Solucion[k]=j+1;
	// 		if(Lugar(k,Solucion)==true){
	// 			contador++;
	// 			ok.push_back(j+1);
	// 		}
	// 	}

	// 	if(contador==0){
	// 		break;
	// 	}

    //     columna = ok[rand()%ok.size()];
    //     Solucion[k] = columna;
    //     ok.clear();
	// }

	// if(contador==0){
	// 	return false;
	// }else{
	//     return true;
    // }
    vector<int>ok(n);
    int contador=0;
    int col=0;
    int aux=0;
    
    for(int k=0;k<n;k++){
        contador=0;
        for(int j=0;j<n;j++){
            if(Lugar(k,Solucion)==true){
                if(contador+1<n){
                    contador++;
                    ok[contador]=j+1;
                }
            }
        }
        if(contador==0){
            break;
        }

        aux=rand()%contador+1;
        col=ok[aux];
        Solucion[k] = col;
    }
    if(contador==0){
        return false;
    }else{
        return true;
    }
}

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
        cout<<"Solucion numero: "<< i+1<<endl;
        for(int j=0;j<Soluciones[i].size();j++){
            cout<<"Reina --> Fila "<<j+1<<" Columna "<<Soluciones[i][j]<<endl;
        }
        cout<<endl;
    }
}

void imprimir(vector<int> Solucion){
    for(int i=0;i<Solucion.size();i++){
        cout<<"Reina --> Fila "<<i+1<<" Columna "<<Solucion[i]<<endl;
    }
}
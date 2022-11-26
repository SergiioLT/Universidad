#include <iostream>
#include <vector>

using namespace std;

void NKreinas(int i){

    int n;

    cout<<"Introduce el tamaño de las n reinas"<<endl;
    cin>>n;


    int k=0;
    vector<int> Reinas;
    vector< vector< int> > Soluciones;

    Reinas.resize(n);
    Reinas[0] = 0;

    while(k>0){

        Reinas[k] = Reinas[k]+1;
        while(Reinas[k]<=n && Lugar(k,Reinas)==false){
            Reinas[k]=Reinas[k]+1;
        }

        if(Reinas[k]<n){
            if(k==n){
                Soluciones.push_back(Reinas);
            }else{
                k = k+1;
                Reinas[k]=0;
            }
        }else{
            k=k-1;
        }
    }
}

bool Lugar(int k, vector<int> Reinas){
    for(int i=1; i<k-1; i++){
        if( (Reinas[i]==Reinas[k]) || (abs(Reinas[i]- Reinas[k])==abs(i-k))){
            return false;
            }
        return true;
    }
}

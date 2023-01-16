#include <iostream>

using namespace std;

void NKreinasBacktracking(int);
void NKreinasVegas();


int main(){

    int opcion;

    while(opcion!=0){
        system ("clear");
        cout<<"Introduzca la opcion que desea usar."<<endl;
        cout<<"1.- Realizar el metodo Backtracking con todas las soluciones"<<endl;
        cout<<"2.- Realizar el metodo Backtracking con la primera solucion"<<endl;
        cout<<"1.- Realizar el metodo Vegas"<<endl;
        cout<<"0.- Salir del programa"<<endl<<endl;
        cin>>opcion;
        //cout<<endl;

        switch(opcion){
            case 1:
                NKreinasBacktracking(1);
            break;
            case 2:
                NKreinasBacktracking(2);
            break;
            case 3:
                NKreinasVegas();
            break;
        }
    }

    return 0;
}   
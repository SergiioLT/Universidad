#include <iostream>

using namespace std;

void nReinasBacktracking(int);
void nReinasVegas();


int main(){

    int opcion;

    while(opcion!=0){
        system ("clear");
        cout<<"Introduzca la opcion que desea usar."<<endl;
        cout<<"1.- N Reinas mediante backtracking - Todas las soluciones"<<endl;
        cout<<"2.- N Reinas mediante backtracking - Primera solucion"<<endl;
        cout<<"3.- N Reinas mediante el metodo de Las Vegas"<<endl;
        cout<<"0.- Salir del programa"<<endl<<endl;
        cin>>opcion;

        switch(opcion){
            case 1:
                nReinasBacktracking(1);
            break;
            case 2:
                nReinasBacktracking(2);
            break;
            case 3:
                nReinasVegas();
            break;
        }
    }

    return 0;
}   
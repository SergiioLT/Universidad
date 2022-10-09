// #include "determinanteRecursivo.cpp"
// #include "determinanteIterativo.cpp"
// #include "ordenacionQuickSort.cpp"
#include <iostream>


using namespace std;

void ordenacionQuickSort();
void determinanteIterativo();
void determinanteRecursivo();

int main(){

    int opcion;

    while(opcion!=0){
        cout<<"Introduzca la opcion que desea usar."<<endl;
        cout<<"1.- Ordenacion QuickSort"<<endl;
        cout<<"2.- Determinante iterativo"<<endl;
        cout<<"3.- Determinante recursivo"<<endl;
        cout<<"0.- Salir del programa"<<endl<<endl;
        cin>>opcion;
        cout<<endl;

        switch(opcion){
            case 1:
                ordenacionQuickSort();
            break;

            case 2:
                determinanteIterativo();
            break;

            case 3:
                determinanteRecursivo();
            break;
        }
    }

    return 0;
}   
//#include "determinanteRecursivo.cpp"
//#include "determinanteIterativo.cpp"
#include "ordenacionQuickSort.cpp"
#include <iostream>


using namespace std;

int main(){

    int opcion;

    while(opcion!=0){
        cout<<"Introduzca la opcion que desea usar."<<endl;
        cout<<"1.- Ordenacion QuickSort"<<endl;
        cout<<"2.- Producto matrices cuadradas"<<endl;
        cout<<"3.- Fibonacci recursivos"<<endl;
        cout<<"0.- Salir del programa"<<endl<<endl;
        cin>>opcion;
        cout<<endl;

        switch(opcion){
            case 1:
                ordenacionQuickSort();
            break;

            case 2:
                //productoMatricesCuadradas();
            break;

            case 3:
                //fibonacciRecursivo();
            break;
        }
    }

    return 0;
}   
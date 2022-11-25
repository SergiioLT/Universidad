#include <iostream>
#include <stdlib.h>


using namespace std;

void segmentacionmetprimero();
void segmentacionmetsegundo();

int main(){

    int opcion;

    while(opcion!=0){
        system ("clear");
        cout<<"Introduzca la opcion que desea usar."<<endl;
        cout<<"1.- Segmentacion metodo 1"<<endl;
        cout<<"2.- Segmentacion metodo 2"<<endl;
        cout<<"0.- Salir del programa"<<endl<<endl;
        cin>>opcion;
        //cout<<endl;

        switch(opcion){
            case 1:
                segmentacionmetprimero();
            break;

            case 2:
                //segmentacionmetsegundo();
            break;
        }
    }

    return 0;
}   
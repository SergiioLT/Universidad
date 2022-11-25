#include <iostream>

using namespace std;

void NKreinas(int);

int main(){

    int opcion;

    while(opcion!=0){
        system ("clear");
        cout<<"Introduzca la opcion que desea usar."<<endl;
        cout<<"1.- "<<endl;
        cout<<"0.- Salir del programa"<<endl<<endl;
        cin>>opcion;
        //cout<<endl;

        switch(opcion){
            case 1:
                NKreinas(1);
            break;
            case 2
                NKreinas(2);
            break;
        }
    }

    return 0;
}   
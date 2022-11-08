#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    vector < int > posicion;
    vector < long double > error;

    for(int i=0;i<10;i++){
        posicion.push_back(i);
        error.push_back(i);
    }
    
      for(int i=0;i<10;i++){
        cout<<" Poisicion i: "<<i<< " valor "<<posicion[i];
        cout<<"                    Poisicion error i: "<<i<< " valor "<<error[i]<<endl;
    }

    posicion.erase(posicion.begin()+2);
    error.erase(error.begin()+2);
    cout<<endl;

    for(int i=0;i<10;i++){
        cout<<" Poisicion i: "<<i<< " valor "<<posicion[i];
        cout<<"                    Poisicion error i: "<<i<< " valor "<<error[i]<<endl;
    }

    return 0;
}

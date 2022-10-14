#include <iostream>


using namespace std;

double *FuncionaPlease(){
    double *minValues[5];
    
    for(int i=0;i<5;i++){
        minValues[i] = new double(i);
    }

    for(int i=0; i<5;i++){
        cout<<"Valor en la funcion: "<<i<< " "<< *minValues[i]<<endl;
    }

    return *minValues;
}

int main(int argc, char const *argv[])
{
    double *testValues;
    testValues = FuncionaPlease();
    for(int i=0; i<5;i++){
        cout<<"Valor : "<<testValues[i]<<endl;
    }
    return 0;
}

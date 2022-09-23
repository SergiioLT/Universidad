#include<iostream>
#include<vector>
using namespace std;

// last element is taken as pivot
int Partition(vector<int> &v, int start, int end){
	
	int pivot = end;
	int j = start;
	for(int i=start;i<end;++i){
		if(v[i]<v[pivot]){
			swap(v[i],v[j]);
			++j;
		}
	}
	swap(v[j],v[pivot]);
	return j;
	
}

void Quicksort(vector<int> &v, int start, int end ){

	if(start<end){
		int p = Partition(v,start,end);
		Quicksort(v,start,p-1);
		Quicksort(v,p+1,end);
	}
	
}

void PrintVector(vector<int> v){
	for(int i=0;i<v.size();++i)
		cout<<v[i]<<" ";
	cout<<"\n\n";
}

void rellenarVector(vector<int> &v){
    for(size_t i=0;i<v.size();i++){
        v[i] = rand()%100;
    }
}

int main() {
	
    srand(time(NULL));

	vector<int> v;
	v.resize(10);
	rellenarVector(v);
	
	cout<<"Vector Before Sorting: "<<endl;
	PrintVector(v);
	
	Quicksort(v,0,v.size()-1);
	
	cout<<"Vector After Sorting: "<<endl;
	PrintVector(v);
		
}
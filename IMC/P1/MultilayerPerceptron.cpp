/*********************************************************************
* File  : MultilayerPerceptron.cpp
* Date  : 2020
*********************************************************************/

#include "MultilayerPerceptron.h"

#include "util.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <math.h>


using namespace imc;
using namespace std;
using namespace util;

// ------------------------------
// Constructor: Default values for all the parameters
MultilayerPerceptron::MultilayerPerceptron()
{
	eta = 0.1;
	mu = 0.9;

	nOfLayers = 1;
	layers = NULL;


}

// ------------------------------
// Allocate memory for the data structures
// nl is the number of layers and npl is a vetor containing the number of neurons in every layer
// Give values to Layer* layers
int MultilayerPerceptron::initialize(int nl, int npl[]) {
	
	layers = new Layer[nl];
	nOfLayers = nl;

	if(layers ==NULL){
		cout<<"Error en MultilayerPerceptron::initialize"<<endl;
		return -1;
	}

	for(int i=0; i<nl;i++){
		layers[i].nOfNeurons=npl[i];
		layers[i].neurons = new Neuron[npl[i]];

		for(int j=0; j<layers[i].nOfNeurons; j++){
			if(i>0){ //+1 = w0
				layers[i].neurons[j].w = new double[layers[i-1].nOfNeurons +1];
				layers[i].neurons[j].wCopy = new double[layers[i-1].nOfNeurons +1];
				layers[i].neurons[j].deltaW = new double[layers[i-1].nOfNeurons +1];
				layers[i].neurons[j].lastDeltaW = new double[layers[i-1].nOfNeurons +1];
			}
		}
	}
	return 0;

}

// ------------------------------
// DESTRUCTOR: free memory
MultilayerPerceptron::~MultilayerPerceptron() {
	freeMemory();
	cout<<"free memory"<<endl;
}

// ------------------------------
// Free memory for the data structures
void MultilayerPerceptron::freeMemory() {
	
	for(int i=0; i<nOfLayers; i++){
		for(int j=0 ; j<layers[i].nOfNeurons; j++){
			if(i>0){ //+1 because w0
				delete[] layers[i].neurons[j].w ;
				delete[] layers[i].neurons[j].wCopy ;
				delete[] layers[i].neurons[j].deltaW ;
				delete[] layers[i].neurons[j].lastDeltaW ;
			}
		}
	}
	delete[] layers[0].neurons;
	delete[] layers;
}

// ------------------------------
// Feel all the weights (w) with random numbers between -1 and +1
void MultilayerPerceptron::randomWeights() {

	for(int i=1; i<nOfLayers; i++){
		for(int j=0; j<layers[i].nOfNeurons; j++){
			for(int k=0; k<layers[i-1].nOfNeurons+1; k++){ 
				layers[i].neurons[j].w[k] = randomDouble(-1,1);
			}
		}
	}

}

// ------------------------------
// Feed the input neurons of the network with a vector passed as an argument
void MultilayerPerceptron::feedInputs(double* input) {

	for(int i=0; i<layers[0].nOfNeurons; i++){ 
		layers[0].neurons[i].out = input[i];
	}

}

// ------------------------------
// Get the outputs predicted by the network (out vector the output layer) and save them in the vector passed as an argument
void MultilayerPerceptron::getOutputs(double* output)
{

	for(int i=0; i< layers[nOfLayers-1].nOfNeurons; i++){
		output[i] = layers[nOfLayers-1].neurons[i].out;
	}

}

// ------------------------------
// Make a copy of all the weights (copy w in wCopy)
void MultilayerPerceptron::copyWeights() {

	for(int i=1; i < nOfLayers; i++){
		for(int j=0; j< layers[i].nOfNeurons; j++){
			for(int k=0; k< layers[i-1].nOfNeurons + 1; k++){ 
				layers[i].neurons[j].wCopy[k] = layers[i].neurons[j].w[k];
			}
		}
	}

}

// ------------------------------
// Restore a copy of all the weights (copy wCopy in w)
void MultilayerPerceptron::restoreWeights() {
	
	for(int i=1; i < nOfLayers; i++){
		for(int j=0; j< layers[i].nOfNeurons; j++){
			for(int k=0; k< layers[i-1].nOfNeurons + 1; k++){ 
				layers[i].neurons[j].w[k] = layers[i].neurons[j].wCopy[k];
			}
		}
	}

}

// ------------------------------
// Calculate and propagate the outputs of the neurons, from the first layer until the last one -->-->
void MultilayerPerceptron::forwardPropagate() {
	
	for(int i=1; i<nOfLayers; i++){
		for(int j=0; j<layers[i].nOfNeurons; j++){
			double net = 0.0;
			net += layers[i].neurons[j].w[0];

			for(int k=1; k<layers[i-1].nOfNeurons+1;k++){
				net += layers[i].neurons[j].w[k] * layers[i-1].neurons[k-1].out; //Net <- wj0 + sum (wji (layer) * out (layer -1))
			}
			
			layers[i].neurons[j].out = 1.0/(1+ exp(-net));
		}
	}

}

// ------------------------------
// Obtain the output error (MSE) of the out vector of the output layer wrt a target vector and return it
double MultilayerPerceptron::obtainError(double* target) {
	
	int size = layers[nOfLayers-1].nOfNeurons;
	double* pred = new double[size];
	double mse = 0.0;
	getOutputs(pred);

	for(int i=0; i< size; i++){
		mse += pow((pred[i] - target[i]),2);
	}

	delete[] pred;
	
	return (mse/layers[nOfLayers-1].nOfNeurons);

}

// ------------------------------
// Backpropagate the output error wrt a vector passed as an argument, from the last layer to the first one <--<--
void MultilayerPerceptron::backpropagateError(double* target) {
	
	for(int i=0; i<layers[nOfLayers-1].nOfNeurons; i++){ //We start in the last layer to calculate delta for the rest layers
		double out = layers[nOfLayers-1].neurons[i].out;
		layers[nOfLayers-1].neurons[i].delta = -(target[i]-out)*out*(1-out);
	}

	for(int i=nOfLayers-2; i>=1; i--){ 
		for(int j=0; j<layers[i].nOfNeurons; j++){
			double out = layers[i].neurons[j].out;
			double sum = 0.0;
			for(int k=0; k<layers[i+1].nOfNeurons;k++){
				sum += layers[i+1].neurons[k].w[j+1]*layers[i+1].neurons[k].delta;
			}
			layers[i].neurons[j].delta = sum*out*(1-out);
		}
	}

}

// ------------------------------
// Accumulate the changes produced by one pattern and save them in deltaW
void MultilayerPerceptron::accumulateChange() {

	for(int i=1; i<nOfLayers; i++){
		for(int j=0; j<layers[i].nOfNeurons; j++){
			for(int k=1; k<layers[i-1].nOfNeurons+1; k++){
				layers[i].neurons[j].deltaW[k] += layers[i].neurons[j].delta * layers[i-1].neurons[k-1].out;
			}
			layers[i].neurons[j].deltaW[0] += layers[i].neurons[j].delta;
		}
	}

}

// ------------------------------
// Update the network weights, from the first layer to the last one
void MultilayerPerceptron::weightAdjustment() {

	for(int i=1; i<nOfLayers; i++){
		for(int j=0; j<layers[i].nOfNeurons; j++){
			for(int k=1; k<layers[i-1].nOfNeurons+1;k++){
				layers[i].neurons[j].w[k] = layers[i].neurons[j].w[k] - eta*layers[i].neurons[j].deltaW[k] - mu* (eta*layers[i].neurons[j].lastDeltaW[k]);
			}
			layers[i].neurons[j].w[0] = layers[i].neurons[j].w[0] - eta*layers[i].neurons[j].deltaW[0] - mu* (eta*layers[i].neurons[j].lastDeltaW[0]);
		}
	}

}

// ------------------------------
// Print the network, i.e. all the weight matrices
void MultilayerPerceptron::printNetwork() {

	for(int i=1; i < nOfLayers; i++){	
		cout<<"Layer "<<i<<endl;
		cout<<"------"<<endl;

		for(int j=0; j< layers[i].nOfNeurons; j++){
			for(int k=0; k< layers[i-1].nOfNeurons +1; k++){
				cout<<layers[i].neurons[j].w[k]<< " ";
			}

		cout<<endl;
		}
	}

}

// ------------------------------
// Perform an epoch: forward propagate the inputs, backpropagate the error and adjust the weights
// input is the input vector of the pattern and target is the desired output vector of the pattern
void MultilayerPerceptron::performEpochOnline(double* input, double* target) {
	//Clearing deltaW in case of trash values
	for(int i=1; i<nOfLayers; i++){
		for(int j=0; j<layers[i].nOfNeurons; j++){
			for(int k=0; k<layers[i-1].nOfNeurons +1; k++){
				layers[i].neurons[j].deltaW[k] = 0.0;
			}
		}
	}
	feedInputs(input);
	forwardPropagate();
	backpropagateError(target);
	accumulateChange();
	weightAdjustment();
}

// ------------------------------
// Perform an online training for a specific trainDataset
void MultilayerPerceptron::trainOnline(Dataset* trainDataset) {
	int i;
	for(i=0; i<trainDataset->nOfPatterns; i++){
		performEpochOnline(trainDataset->inputs[i], trainDataset->outputs[i]);
	}
}

// ------------------------------
// Test the network with a dataset and return the MSE
double MultilayerPerceptron::test(Dataset* testDataset) {
	double error = 0.0;

	for(int i=0; i< testDataset->nOfPatterns; i++){
		feedInputs(testDataset->inputs[i]);
		forwardPropagate();
		error = obtainError(testDataset->outputs[i]);
	}
	return (error/testDataset->nOfPatterns);
}


// Optional - KAGGLE
// Test the network with a dataset and return the MSE
// Your have to use the format from Kaggle: two columns (Id y predictied)
void MultilayerPerceptron::predict(Dataset* pDatosTest)
{
	int i;
	int j;
	int numSalidas = layers[nOfLayers-1].nOfNeurons;
	double * obtained = new double[numSalidas];
	
	cout << "Id,Predicted" << endl;
	
	for (i=0; i<pDatosTest->nOfPatterns; i++){

		feedInputs(pDatosTest->inputs[i]);
		forwardPropagate();
		getOutputs(obtained);
		
		cout << i;

		for (j = 0; j < numSalidas; j++)
			cout << "," << obtained[j];
		cout << endl;

	}
}

// ------------------------------
// Run the traning algorithm for a given number of epochs, using trainDataset
// Once finished, check the performance of the network in testDataset
// Both training and test MSEs should be obtained and stored in errorTrain and errorTest
void MultilayerPerceptron::runOnlineBackPropagation(Dataset * trainDataset, Dataset * pDatosTest, int maxiter, double *errorTrain, double *errorTest)
{
	int countTrain = 0;
	// Random assignment of weights (starting point)
	randomWeights();
	double minTrainError = 0;
	int iterWithoutImproving;
	double testError = 0;

	// Learning
	do {
		trainOnline(trainDataset);
		double trainError = test(trainDataset);
		if(countTrain==0 || trainError < minTrainError){
			minTrainError = trainError;
			copyWeights();
			iterWithoutImproving = 0;
		}
		else if( (trainError-minTrainError) < 0.00001)
			iterWithoutImproving = 0;
		else
			iterWithoutImproving++;

		if(iterWithoutImproving==50){
			cout << "We exit because the training is not improving!!"<< endl;
			restoreWeights();
			countTrain = maxiter;
		}


		countTrain++;

		cout << "Iteration " << countTrain << "\t Training error: " << trainError << endl;

	} while ( countTrain<maxiter );

	cout << "NETWORK WEIGHTS" << endl;
	cout << "===============" << endl;
	printNetwork();

	cout << "Desired output Vs Obtained output (test)" << endl;
	cout << "=========================================" << endl;
	for(int i=0; i<pDatosTest->nOfPatterns; i++){
		double* prediction = new double[pDatosTest->nOfOutputs];

		// Feed the inputs and propagate the values
		feedInputs(pDatosTest->inputs[i]);
		forwardPropagate();
		getOutputs(prediction);
		for(int j=0; j<pDatosTest->nOfOutputs; j++)
			cout << pDatosTest->outputs[i][j] << " -- " << prediction[j] << " ";
		cout << endl;
		delete[] prediction;

	}

	testError = test(pDatosTest);
	*errorTest=testError;
	*errorTrain=minTrainError;

}

// Optional Kaggle: Save the model weights in a textfile
bool MultilayerPerceptron::saveWeights(const char * archivo)
{
	// Object for writing the file
	ofstream f(archivo);

	if(!f.is_open())
		return false;

	// Write the number of layers and the number of layers in every layer
	f << nOfLayers;

	for(int i = 0; i < nOfLayers; i++)
		f << " " << layers[i].nOfNeurons;
	f << endl;

	// Write the weight matrix of every layer
	for(int i = 1; i < nOfLayers; i++)
		for(int j = 0; j < layers[i].nOfNeurons; j++)
			for(int k = 0; k < layers[i-1].nOfNeurons + 1; k++)
				f << layers[i].neurons[j].w[k] << " ";

	f.close();

	return true;

}


// Optional Kaggle: Load the model weights from a textfile
bool MultilayerPerceptron::readWeights(const char * archivo)
{
	// Object for reading a file
	ifstream f(archivo);

	if(!f.is_open())
		return false;

	// Number of layers and number of neurons in every layer
	int nl;
	int *npl;

	// Read number of layers
	f >> nl;

	npl = new int[nl];

	// Read number of neurons in every layer
	for(int i = 0; i < nl; i++)
		f >> npl[i];

	// Initialize vectors and data structures
	initialize(nl, npl);

	// Read weights
	for(int i = 1; i < nOfLayers; i++)
		for(int j = 0; j < layers[i].nOfNeurons; j++)
			for(int k = 0; k < layers[i-1].nOfNeurons + 1; k++)
				f >> layers[i].neurons[j].w[k];

	f.close();
	delete[] npl;

	return true;
}

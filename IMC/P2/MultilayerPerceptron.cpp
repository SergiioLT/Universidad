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
#include <cstdlib>  // To establish the seed srand() and generate pseudorandom numbers rand()
#include <limits>
#include <math.h>


using namespace imc;
using namespace std;
using namespace util;

// ------------------------------
// Constructor: Default values for all the parameters
MultilayerPerceptron::MultilayerPerceptron()
{
	
	eta = 0.7;
	mu = 1;

	nOfLayers = 1;
	layers = NULL;
	
	online = false;
	outputFunction = 0;

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
// Fill all the weights (w) with random numbers between -1 and +1
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
// Get the outputs predicted by the network (out vector of the output layer) and save them in the vector passed as an argument
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

	double net, sumNet;

	for(int i=1; i < nOfLayers; i++){
		sumNet = 0.0;

		for(int j=0; j<layers[i].nOfNeurons; j++){
			net= 0.0;
			for(int k=1; k<layers[i-1].nOfNeurons+1;k++){
				net += layers[i].neurons[j].w[k] * layers[i-1].neurons[k-1].out;
			}

			net += layers[i].neurons[j].w[0];

			if((i==nOfLayers-1) && (outputFunction == 1)){ //Softmax
				layers[i].neurons[j].out = exp(net);
				sumNet += exp(net);
			}else{ //Same from last assignment
				layers[i].neurons[j].out = 1 / (1+exp(-net));
			}
		}

		if((i == nOfLayers-1) && (outputFunction==1)){  //Softmax
			for(int j=0;j<layers[i].nOfNeurons+1; j++){
				layers[i].neurons[j].out /= sumNet;
			}
		}

	}

}

// ------------------------------
// Obtain the output error (MSE) of the out vector of the output layer wrt a target vector and return it
// errorFunction=1 => Cross Entropy // errorFunction=0 => MSE
double MultilayerPerceptron::obtainError(double* target, int errorFunction) {
	
	int size = layers[nOfLayers-1].nOfNeurons;
	double* pred = new double[size];
	getOutputs(pred);

	if(errorFunction == 0){ //MSE

		double mse = 0.0;
		for(int i=0; i< size; i++){
			mse += pow((pred[i] - target[i]),2);
		}
		return (mse/size);

	}else{

		double cen = 0.0;
		for(int i=0; i<size;i++){
			cen += (target[i]*log(pred[i]));
		}
		return (cen/size);

	}

}


// ------------------------------
// Backpropagate the output error wrt a vector passed as an argument, from the last layer to the first one <--<--
// errorFunction=1 => Cross Entropy // errorFunction=0 => MSE
void MultilayerPerceptron::backpropagateError(double* target, int errorFunction) {

	double sum = 0.0;

	if(outputFunction==0){ 	//Lab assignment 1
		if(errorFunction==0){
			for(int i=0 ; i<layers[nOfLayers-1].nOfNeurons; i++){ 
				layers[nOfLayers-1].neurons[i].delta = (-2)*(target[i]-layers[nOfLayers-1].neurons[i].out)*(layers[nOfLayers-1].neurons[i].out*(1-layers[nOfLayers-1].neurons[i].out));	
			}
		}else{
			for(int i=0;i<layers[nOfLayers-1].nOfNeurons;i++){
				layers[nOfLayers-1].neurons[i].delta = (-1)*(target[i] / layers[nOfLayers-1].neurons[i].out)*(layers[nOfLayers-1].neurons[i].out*(1 - layers[nOfLayers-1].neurons[i].out)); 		
			}
		}

	}else{ //Softmax
		int softCase = 0;
		for(int i=0; i<layers[nOfLayers-1].nOfNeurons;i++){
			sum = 0.0;
			for(int j=0; j<layers[nOfLayers-1].nOfNeurons;j++){
				if(j==i){
					softCase = 1;
				}else{
					softCase = 0;
				}
				if(errorFunction==0){
					sum += ((target[j] - layers[nOfLayers-1].neurons[j].out) * layers[nOfLayers-1].neurons[i].out * (softCase - layers[nOfLayers-1].neurons[j].out));
				}else{
					sum += ((target[j] / layers[nOfLayers-1].neurons[j].out) * layers[nOfLayers-1].neurons[i].out * (softCase - layers[nOfLayers-1].neurons[j].out));
				}
			}	

			layers[nOfLayers-1].neurons[i].delta = -sum;
		}
	}

	for(int i=nOfLayers-2; i>=1; i--){ //Lab assigment 1
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

	if(online==true){
		for(int i=1; i<nOfLayers; i++){
			for(int j=0; j<layers[i].nOfNeurons; j++){
				for(int k=1; k<layers[i-1].nOfNeurons+1;k++){
					layers[i].neurons[j].w[k] = layers[i].neurons[j].w[k] - eta*layers[i].neurons[j].deltaW[k] - mu* (eta*layers[i].neurons[j].lastDeltaW[k]);
				}
				layers[i].neurons[j].w[0] = layers[i].neurons[j].w[0] - eta*layers[i].neurons[j].deltaW[0] - mu* (eta*layers[i].neurons[j].lastDeltaW[0]);
			}
		}
	}else{
		for(int i=1; i<nOfLayers; i++){
			for(int j=0; j<layers[i].nOfNeurons; j++){
				for(int k=1; k<layers[i-1].nOfNeurons+1;k++){
					layers[i].neurons[j].w[k] = layers[i].neurons[j].w[k] - ((eta*layers[i].neurons[j].deltaW[k])/nOfTrainingPatterns) - ((mu* (eta*layers[i].neurons[j].lastDeltaW[k]))/nOfTrainingPatterns);
				}
				layers[i].neurons[j].w[0] = layers[i].neurons[j].w[0] - ((eta*layers[i].neurons[j].deltaW[0])/nOfTrainingPatterns) - ((mu* (eta*layers[i].neurons[j].lastDeltaW[0]))/nOfTrainingPatterns);
			}
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
// The step of adjusting the weights must be performed only in the online case
// If the algorithm is offline, the weightAdjustment must be performed in the "train" function
// errorFunction=1 => Cross Entropy // errorFunction=0 => MSE
void MultilayerPerceptron::performEpoch(double* input, double* target, int errorFunction) {
	if(online==true){
		for(int i=1; i<nOfLayers; i++){
			for(int j=0; j<layers[i].nOfNeurons; j++){
				for(int k=0; k<layers[i-1].nOfNeurons +1; k++){
					layers[i].neurons[j].deltaW[k] = 0.0;
				}
			}
		}
	}

	feedInputs(input);
	forwardPropagate();
	backpropagateError(target, errorFunction);
	accumulateChange();

	if(online == true){
		weightAdjustment();
	}
}

// ------------------------------
// Train the network for a dataset (one iteration of the external loop)
// errorFunction=1 => Cross Entropy // errorFunction=0 => MSE
void MultilayerPerceptron::train(Dataset* trainDataset, int errorFunction) {
	if(online == false){
		for(int i=1; i<nOfLayers; i++){
			for(int j=0; j<layers[i].nOfNeurons; j++){
				for(int k=0; k<layers[i-1].nOfNeurons +1; k++){
					layers[i].neurons[j].deltaW[k] = 0.0;
				}
			}
		}
	}

	for(int i=0; i<trainDataset->nOfPatterns; i++){
		performEpoch(trainDataset->inputs[i], trainDataset->outputs[i], errorFunction);
	}

	if(online == false){
		weightAdjustment();
	}

}

// ------------------------------
// Test the network with a dataset and return the error
// errorFunction=1 => Cross Entropy // errorFunction=0 => MSE
double MultilayerPerceptron::test(Dataset* dataset, int errorFunction) {

	double error = 0.0;
	int size = dataset->nOfPatterns;

	for(int i=0; i < dataset->nOfPatterns;i++){
		feedInputs(dataset->inputs[i]);
		forwardPropagate();
		error += obtainError(dataset->outputs[i], errorFunction);
	}

	if(errorFunction == 0){
		return (error/size);
	}else{
		return (-1*(error/size));
	}

}


// ------------------------------
// Test the network with a dataset and return the CCR
double MultilayerPerceptron::testClassification(Dataset* dataset) {

	double ccr= 0.0, aux= 0.0;

	int outInd=0;
	int desireInd=0;
	

	for(int i=0; i<dataset->nOfPatterns; i++){
		outInd=0;
		desireInd=0;

		feedInputs(dataset->inputs[i]);
		forwardPropagate();

		double* out = new double[dataset->nOfOutputs];
		getOutputs(out);

		for(int j=1; j<dataset->nOfOutputs; j++){
			if(dataset->outputs[i][outInd] < dataset->outputs[i][j]){
				outInd = j;
			}

			if(out[desireInd] < out[j]){
				desireInd = j;
			}
		}

		if(desireInd==outInd){
			aux++;
		}

	}
	ccr= (100*aux / (double)dataset->nOfPatterns);
	return ccr;

}


// ------------------------------
// Optional Kaggle: Obtain the predicted outputs for a dataset
void MultilayerPerceptron::predict(Dataset* dataset)
{
	int i;
	int j;
	int numSalidas = layers[nOfLayers-1].nOfNeurons;
	double * salidas = new double[numSalidas];
	
	cout << "Id,Category" << endl;
	
	for (i=0; i<dataset->nOfPatterns; i++){

		feedInputs(dataset->inputs[i]);
		forwardPropagate();
		getOutputs(salidas);

		int maxIndex = 0;
		for (j = 0; j < numSalidas; j++)
			if (salidas[j] >= salidas[maxIndex])
				maxIndex = j;
		
		cout << i << "," << maxIndex << endl;

	}
}



// ------------------------------
// Run the traning algorithm for a given number of epochs, using trainDataset
// Once finished, check the performance of the network in testDataset
// Both training and test MSEs should be obtained and stored in errorTrain and errorTest
// Both training and test CCRs should be obtained and stored in ccrTrain and ccrTest
// errorFunction=1 => Cross Entropy // errorFunction=0 => MSE
void MultilayerPerceptron::runBackPropagation(Dataset * trainDataset, Dataset * testDataset, int maxiter, double *errorTrain, double *errorTest, double *ccrTrain, double *ccrTest, int errorFunction)
{
	int countTrain = 0;

	// Random assignment of weights (starting point)
	randomWeights();

	double minTrainError = 0;
	int iterWithoutImproving = 0;
	nOfTrainingPatterns = trainDataset->nOfPatterns;


	// Learning
	do {

		train(trainDataset,errorFunction);

		double trainError = test(trainDataset,errorFunction);
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

	if ( iterWithoutImproving!=50)
		restoreWeights();

	cout << "NETWORK WEIGHTS" << endl;
	cout << "===============" << endl;
	printNetwork();

	cout << "Desired output Vs Obtained output (test)" << endl;
	cout << "=========================================" << endl;
	for(int i=0; i<testDataset->nOfPatterns; i++){
		double* prediction = new double[testDataset->nOfOutputs];

		// Feed the inputs and propagate the values
		feedInputs(testDataset->inputs[i]);
		forwardPropagate();
		getOutputs(prediction);
		for(int j=0; j<testDataset->nOfOutputs; j++)
			cout << testDataset->outputs[i][j] << " -- " << prediction[j] << " ";
		cout << endl;
		delete[] prediction;

	}

	*errorTest=test(testDataset,errorFunction);;
	*errorTrain=minTrainError;
	*ccrTest = testClassification(testDataset);
	*ccrTrain = testClassification(trainDataset);

}

// -------------------------
// Optional Kaggle: Save the model weights in a textfile
bool MultilayerPerceptron::saveWeights(const char * fileName)
{
	// Object for writing the file
	ofstream f(fileName);

	if(!f.is_open())
		return false;

	// Write the number of layers and the number of layers in every layer
	f << nOfLayers;

	for(int i = 0; i < nOfLayers; i++)
	{
		f << " " << layers[i].nOfNeurons;
	}
	f << " " << outputFunction;
	f << endl;

	// Write the weight matrix of every layer
	for(int i = 1; i < nOfLayers; i++)
		for(int j = 0; j < layers[i].nOfNeurons; j++)
			for(int k = 0; k < layers[i-1].nOfNeurons + 1; k++)
				if(layers[i].neurons[j].w!=NULL)
				    f << layers[i].neurons[j].w[k] << " ";

	f.close();

	return true;

}


// -----------------------
// Optional Kaggle: Load the model weights from a textfile
bool MultilayerPerceptron::readWeights(const char * fileName)
{
	// Object for reading a file
	ifstream f(fileName);

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
	{
		f >> npl[i];
	}
	f >> outputFunction;

	// Initialize vectors and data structures
	initialize(nl, npl);

	// Read weights
	for(int i = 1; i < nOfLayers; i++)
		for(int j = 0; j < layers[i].nOfNeurons; j++)
			for(int k = 0; k < layers[i-1].nOfNeurons + 1; k++)
				if(!(outputFunction==1 && (i==(nOfLayers-1)) && (k==(layers[i].nOfNeurons-1))))
					f >> layers[i].neurons[j].w[k];

	f.close();
	delete[] npl;

	return true;
}

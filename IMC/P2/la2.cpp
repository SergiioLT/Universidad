//============================================================================
// Introduction to computational models
// Name        : la2.cpp
// Author      : Pedro A. Gutiérrez
// Version     :
// Copyright   : Universidad de Córdoba
//============================================================================


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <ctime>    // To obtain current time time()
#include <cstdlib>  // To establish the seed srand() and generate pseudorandom numbers rand()
#include <string.h>
#include <math.h>
#include <float.h>

#include "MultilayerPerceptron.h"
#include "util.h"


using namespace imc;
using namespace std;

int main(int argc, char **argv) {
	// Process the command line
    bool  tflag = 0, Tflag = 0, wflag = 0, pflag =0,iflag = 0, lflag = 0, hflag = 0, eflag = 0, mflag = 0, oflag = false, fflag = 0, sflag = 0, nflag = false;
    char *TestFilename = NULL, *TrainFilename = NULL,*wvalue = NULL;
    int c;
    int numIteration = 1000, neuroLayer = 5, hiddenLayer = 1, fvalue=0, nvalue=0, svalue=0, ovalue=0;
    double eta = 0.7, mu = 1;

    opterr = 0;

    // a: Option that requires an argument
    // a:: The argument required is optional
    while ((c = getopt(argc, argv, "t:T:w:i:l:h:e:m:o:f:s:n")) != -1)
    {

        // The parameters needed for using the optional prediction mode of Kaggle have been included.
        // You should add the rest of parameters needed for the lab assignment.
        switch(c){
            case 'T':
                Tflag = true;
                TestFilename = optarg;
                break;
            case 'w':
                wflag = true;
                wvalue = optarg;
                break;
            case 'p':
                pflag = true;
                break;
            case 't':
                tflag = true;
                TrainFilename = optarg;
                break;
            case 'i':
                iflag = true;
                numIteration = atoi(optarg);
                break;
            case 'l':
                lflag = true;
                hiddenLayer = atoi(optarg);
                break;
            case 'h':
                hflag = true;
                neuroLayer = atoi(optarg);
                break;
            case 'e':
                eflag = true;
                eta = atof(optarg);
                break;
            case 'm':
                mflag = true;
                mu = atof(optarg);
                break;
            case 'o':
                oflag = true;
                ovalue = 1;
                break;
            case 'f':
                fflag = true;
                fvalue = atoi(optarg);
                break;
            case 's':
                sflag = true;
                svalue = 1;
                break;
            case 'n':
                nflag = true;
                nvalue = 1;
                break;
            case '?':
                if (optopt == 'T' || optopt == 'w' || optopt == 't' || optopt == 'p' || optopt == 'i' || optopt == 'l' || optopt == 'h' || optopt == 'e' || optopt == 'm' || optopt == 'o' || optopt == 'f' || optopt == 's' || optopt == 'n')
                    fprintf (stderr, "The option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown character `\\x%x'.\n",
                             optopt);
                return EXIT_FAILURE;
            default:
                return EXIT_FAILURE;
        }
    }


    if (!pflag) {
        //////////////////////////////////
        // TRAINING AND EVALUATION MODE //
        //////////////////////////////////

        // Multilayer perceptron object
    	MultilayerPerceptron mlp;

    	// Type of error considered
    	int error=fvalue; 

    	// Maximum number of iterations
    	int maxIter = numIteration;
        mlp.eta = eta;
        mlp.mu = mu;
        mlp.online= ovalue;
        mlp.outputFunction=svalue;


        // Read training and test data: call to util::readData(...)
    	Dataset * trainDataset;
    	Dataset * testDataset;
        double minTrainOutput, maxOutput;

      if(nflag==0){
            trainDataset = readData(TrainFilename); 
            testDataset = readData(TestFilename);
        }else if(nvalue==1 && sflag == 0){
            cout<<"Normalize non-classification"<<endl;
            trainDataset = readData(TrainFilename); 
            testDataset = readData(TestFilename);
            double minTrainInput[trainDataset->nOfPatterns], maxTrainInput[trainDataset->nOfPatterns];

            printDataset(trainDataset, trainDataset->nOfPatterns);
            minDatasetInputs(trainDataset, minTrainInput);
            maxDatasetInputs(trainDataset, maxTrainInput);

            minTrainOutput = minDatasetOutputs(trainDataset);
            maxOutput =  maxDatasetOutputs(trainDataset);

            minMaxScalerDataSetInputs(trainDataset, -1, 1, minTrainInput ,maxTrainInput);            
            minMaxScalerDataSetOutputs(trainDataset, 0, 1, minTrainOutput, maxOutput);
            
            minMaxScalerDataSetInputs(testDataset, -1, 1, minTrainInput, maxTrainInput);
            minMaxScalerDataSetOutputs(testDataset, 0, 1, minTrainOutput, maxOutput);

            printDataset(trainDataset,trainDataset->nOfPatterns);

        }else if(nvalue==1 && sflag ==true){
            cout<<"Normalize classification"<<endl;
            trainDataset = readData(TrainFilename); 
            testDataset = readData(TestFilename);
            double minTrainInput[trainDataset->nOfPatterns], maxTrainInput[trainDataset->nOfPatterns];

            printDataset(trainDataset, trainDataset->nOfPatterns);
            minDatasetInputs(trainDataset, minTrainInput);
            maxDatasetInputs(trainDataset, maxTrainInput);

            minMaxScalerDataSetInputs(trainDataset, -1, 1, minTrainInput ,maxTrainInput);                  
            minMaxScalerDataSetInputs(testDataset, -1, 1, minTrainInput, maxTrainInput);

            printDataset(trainDataset,trainDataset->nOfPatterns);
        }


        // Initialize topology vector
    	int layers = hiddenLayer; 
    	int * topology = new int[hiddenLayer+2]; 

        topology[0] = trainDataset->nOfInputs;
        for(int i=1; i<hiddenLayer+1; i++){
            topology[i] = neuroLayer;
        }
        topology[hiddenLayer+1] = trainDataset->nOfOutputs;

        // Initialize the network using the topology vector
        mlp.initialize(layers+2,topology);


		// Seed for random numbers
		int seeds[] = {1,2,3,4,5};
		double *trainErrors = new double[5];
		double *testErrors = new double[5];
		double *trainCCRs = new double[5];
		double *testCCRs = new double[5];
		double bestTestError = DBL_MAX;
		for(int i=0; i<5; i++){
			cout << "**********" << endl;
			cout << "SEED " << seeds[i] << endl;
			cout << "**********" << endl;
			srand(seeds[i]);
			mlp.runBackPropagation(trainDataset,testDataset,maxIter,&(trainErrors[i]),&(testErrors[i]),&(trainCCRs[i]),&(testCCRs[i]),error);
			cout << "We end!! => Final test CCR: " << testCCRs[i] << endl;

			// We save the weights every time we find a better model
			if(wflag && testErrors[i] <= bestTestError)
			{
				mlp.saveWeights(wvalue);
				bestTestError = testErrors[i];
			}
		}


		double trainAverageError = 0, trainStdError = 0;
		double testAverageError = 0, testStdError = 0;
		double trainAverageCCR = 0, trainStdCCR = 0;
		double testAverageCCR = 0, testStdCCR = 0;

        for(int i = 0; i<5; i++){
            trainAverageError += trainErrors[i];
            testAverageError += testErrors[i];
            trainAverageCCR += trainCCRs[i];
            testAverageCCR += testCCRs[i];
        }

        trainAverageError = trainAverageError / 5;
        testAverageError = testAverageError / 5;
        trainAverageCCR = trainAverageCCR / 5;
        testAverageCCR = testAverageCCR / 5;


        for(int i=0; i<5; i++){
            trainStdError += (trainErrors[i] - trainAverageError)*(trainErrors[i] - trainAverageError);
            testStdError += (testErrors[i] - testAverageError)*(testErrors[i] - testAverageError);
            trainStdCCR += (trainCCRs[i] - trainAverageCCR)*(trainCCRs[i] - trainAverageCCR);
            testStdCCR += (testCCRs[i] - testAverageCCR)*(testCCRs[i] - testAverageCCR);
        }

        trainStdError = sqrt(trainStdError/5);
        testStdError = sqrt(testStdError/5);
        trainStdCCR = sqrt(trainStdCCR/5);
        testStdCCR = sqrt(testStdCCR/5);


        // Obtain training and test averages and standard deviations

		cout << "WE HAVE FINISHED WITH ALL THE SEEDS" << endl;

		cout << "FINAL REPORT" << endl;
		cout << "*************" << endl;
	    cout << "Train error (Mean +- SD): " << trainAverageError << " +- " << trainStdError << endl;
	    cout << "Test error (Mean +- SD): " << testAverageError << " +- " << testStdError << endl;
	    cout << "Train CCR (Mean +- SD): " << trainAverageCCR << " +- " << trainStdCCR << endl;
	    cout << "Test CCR (Mean +- SD): " << testAverageCCR << " +- " << testStdCCR << endl;
		return EXIT_SUCCESS;
    } else {

        //////////////////////////////
        // PREDICTION MODE (KAGGLE) //
        //////////////////////////////

        // You do not have to modify anything from here.
        
        // Multilayer perceptron object
        MultilayerPerceptron mlp;

        // Initializing the network with the topology vector
        if(!wflag || !mlp.readWeights(wvalue))
        {
            cerr << "Error while reading weights, we can not continue" << endl;
            exit(-1);
        }

        // Reading training and test data: call to readData(...)
        Dataset *testDataset;
        testDataset = readData(TestFilename);
        if(testDataset == NULL)
        {
            cerr << "The test file is not valid, we can not continue" << endl;
            exit(-1);
        }

        mlp.predict(testDataset);

        return EXIT_SUCCESS;

	}
}


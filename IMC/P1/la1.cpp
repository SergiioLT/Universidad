//============================================================================
// Introduction to computational models
// Name        : la1.cpp
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

#include "imc/MultilayerPerceptron.h"
#include "imc/util.h"


using namespace imc;
using namespace std;
using namespace util;

int main(int argc, char **argv) {
    // Process arguments of the command line
    bool Tflag = 0, wflag = 0, pflag = 0, tflag = 0, iflag = 0, lflag = 0, hflag = 0, eflag = 0, mflag = 0, sflag = false;
    char *TestFilename = NULL, *TrainFilename = NULL,*wvalue = NULL;
    int c;
    int numIteration = 1000, neuroLayer = 5, hiddenLayer = 1;
    double eta = 0.1, mu = 0.9;

    opterr = 0;

    // a: Option that requires an argument
    // a:: The argument required is optional
    while ((c = getopt(argc, argv, "T:w:p:t:i:l:h:e:m:s")) != -1)
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
            case 's':
                sflag = true;
                break;
            case '?':
                if (optopt == 'T' || optopt == 'w' || optopt == 'p' || optopt == 't' || optopt == 'i' || optopt == 'l' || optopt == 'h'|| optopt == 'e' || optopt == 'm' || optopt == 's')
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

        // Parameters of the mlp. For example, mlp.eta = value;
    	int iterations = numIteration;
        mlp.eta = eta;
        mlp.mu = mu;

        Dataset * trainDataset; 
        Dataset * testDataset;

        double minTrainOutput, maxOutput;



        // Read training and test data: call to util::readData(...)
        if(sflag==false){
            trainDataset = readData(TrainFilename); 
            testDataset = readData(TestFilename);
        }else{
            // trainDataset = readData(TrainFilename); 
            // testDataset = readData(TestFilename);
            // double *minTrainInput, *maxTrainInput;

            // cout<<"Antes de normalizar"<<endl;
            // printDataset(trainDataset,trainDataset->nOfPatterns);

            // //Normalize train dataset
            // // minTrainInput = minDatasetInputs(trainDataset);
            // // maxTrainInput = maxDatasetInputs(trainDataset);
            // cout<<"Antes de normalizar"<<endl;
            // minTrainInput = minDatasetInputs(trainDataset);

            // for(int i=0; i<trainDataset->nOfPatterns;i++){
            //     cout<<"Valor en el main : "<<i <<" "<<minTrainInput[i]<<endl;
            // }

            // maxTrainInput = maxDatasetInputs(trainDataset);

            // minTrainOutput = minDatasetOutputs(trainDataset);
            // maxOutput =  maxDatasetOutputs(trainDataset);

            // minMaxScalerDataSetInputs(trainDataset, -1, 1, minTrainInput ,maxTrainInput);
            // minMaxScalerDataSetInputs(testDataset, 0, 1, minTrainInput, maxTrainInput);
            
            // minMaxScalerDataSetOutputs(trainDataset, -1, 1, minTrainOutput, maxOutput);
            // minMaxScalerDataSetOutputs(testDataset, 0, 1, minTrainOutput, maxOutput);

            // cout<<"Despues de normalizar"<<endl;
            // printDataset(trainDataset,trainDataset->nOfPatterns);

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
        double *testErrors = new double[5];
        double *trainErrors = new double[5];
        double bestTestError = 1;
        for(int i=0; i<5; i++){
            cout << "**********" << endl;
            cout << "SEED " << seeds[i] << endl;
            cout << "**********" << endl;
            srand(seeds[i]);
            //cout<<"a"<<endl;
            mlp.runOnlineBackPropagation(trainDataset,testDataset,iterations,&(trainErrors[i]),&(testErrors[i]));
            cout << "We end!! => Final test error: " << testErrors[i] << endl;

            // We save the weights every time we find a better model
            if(wflag && testErrors[i] <= bestTestError)
            {
                mlp.saveWeights(wvalue);
                bestTestError = testErrors[i];
            }
        }

        cout << "WE HAVE FINISHED WITH ALL THE SEEDS" << endl;

        double averageTestError = 0, stdTestError = 0;
        double averageTrainError = 0, stdTrainError = 0;
        
        // Obtain training and test averages and standard deviations
        for(int i=0; i<5; i++){
            averageTrainError += trainErrors[i];
            averageTestError += testErrors[i];
        }

        averageTrainError = averageTrainError/5;
        averageTestError = averageTestError/5;
        
        for(int i=0; i<5; i++){
            stdTrainError += pow(trainErrors[i] - averageTrainError,2);
            stdTestError += pow(testErrors[i] - averageTestError,2);    
        }

        stdTrainError = sqrt(stdTrainError/5);
        stdTestError = sqrt(stdTestError/5);

        cout << "FINAL REPORT" << endl;
        cout << "************" << endl;
        cout << "Train error (Mean +- SD): " << averageTrainError << " +- " << stdTrainError << endl;
        cout << "Test error (Mean +- SD):          " << averageTestError << " +- " << stdTestError << endl;
        return EXIT_SUCCESS;
    }
    else {

        //////////////////////////////
        // PREDICTION MODE (KAGGLE) //
        //////////////////////////////
        
        // Multilayer perceptron object
        MultilayerPerceptron mlp;

        // Initializing the network with the topology vector
        if(!wflag || !mlp.readWeights(wvalue))
        {
            cerr << "Error while reading weights, we can not continue" << endl;
            exit(-1);
        }

        // Reading training and test data: call to util::readData(...)
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


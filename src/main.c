/* 
This file contains a toy neural network that we will 
modify and improve to match the problem
This neural network is inspire from : 
https://www.cs.bham.ac.uk/~jxb/INC/nn.html
https://github.com/yaspr/nn
this NN will predict if the result of an addition (between two number between 0 and 10 ) is true.
i iterate through the Inputs
j iterate through the Hiddens
k iterate through the Outputs
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "type.h"
#include "nn.h"
#include "image_processing.h"
#include "file_manager.h"

#define NB_MAX_LAYER   50
#define NB_MAX_OUTPUTS 50

int main(int argc, char *argv[])
{

    
    srand(time(NULL));

    char * dirs[] = { "dataset/train/NonDemented", "dataset/train/VeryMildDemented"};
    int counter = 0; 

    // number of images
    for( int i = 0; i < 2; i++ ){
        int num = count_file(dirs[i]);
        printf(" file in %s :  %d\n", dirs[i],num);
    }

    mri_image * dataset = malloc( 100 * sizeof(mri_image) );
    u64 * random_pattern = malloc( 100 * sizeof(u64));
    
    // fill dataset
    int total = load_dataset( dirs, 2, dataset, 50);


    counter = total;


    printf("dataset filled : %llu\n", total);
    
    




    
    //  Initialise The NN
    u64 nb_layers = 5;
    u64 input_size = 480;
    u64 neurons_per_layers[NB_MAX_LAYER] = {480,200,50,10,1,1};
    Layer ** layers = malloc( nb_layers * sizeof(Layer *) );

    f64 expected[NB_MAX_OUTPUTS];

    for(u64 i = 0; i < nb_layers; i++){
        layers[i] = createLayer( neurons_per_layers[i], neurons_per_layers[i+1]);
    }

    for(u64 i = 0; i < nb_layers - 1; i++){
        initLayer( layers[i], neurons_per_layers[i+1] );
    }
    printf(" id ; err\n");

    //  Training
    for( u64 train_id = 0; train_id < 400; train_id++ ){
        f64 cumul_err = 0.0f;
        f64 err = 0.0f; 

       
        //randomize dataset
        for( u64 p = 0 ; p < counter ; p++ ) {    /* randomize order of training patterns */
            random_pattern[p] = p ;
        }

        for( u64 p = 0 ; p < counter - 1 ; p++) {
            u64 np = rand() % ( counter - p) + p;
            
            u64 op = random_pattern[p] ;
            random_pattern[p] = random_pattern[np];
            random_pattern[np] = op;
        }
        
        
        //
        for( u64 np = 0 ; np < counter ; np++ ) {
            u64 p = random_pattern[np];

            
            // fill inputs
            for( u64 i = 0; i < input_size; i++ ){
                layers[0]->neurons[i] = (f64)dataset[p].inputs[i] / 20;
            }

            expected[0] = dataset[p].value;

            // compute
            for(u64 i = 0; i < nb_layers - 1; i++){
                computeLayer( layers[i], layers[i+1] );
            }
            
            //delta
            err = computeOutputDelta( layers[nb_layers - 1] , expected );

            cumul_err += err;


            

            for(u64 i = nb_layers - 2; i > 0; i--){
                computeDelta( layers[i], layers[i+1] );
            }

            for(u64 i = 0; i < nb_layers - 1; i++){
                backpropagate( layers[i], layers[i+1] );
            }
        }
        
        //error
        printf(" %llu; %lf\n", train_id, cumul_err/counter);
        cumul_err = 0;
    }
}


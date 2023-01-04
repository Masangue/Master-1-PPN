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


#include "type.h"
#include "nn.h"
#include "image_processing.h"

#define NB_MAX_LAYER   50
#define NB_MAX_OUTPUTS 50

int main(int argc, char *argv[])
{

    if(argc < 2)
    {
        printf("no input file\n");
        return 1;
    }


    printf(" id ; err\n");
    srand(time(NULL));
    
    f64 cumul_err = 0.0f;

    u64 nb_layers = 4;
    u64 neurons_per_layers[NB_MAX_LAYER] = {3,6,4,1,1};
    Layer ** layers = malloc( nb_layers * sizeof(Layer *) );

    f64 expected[NB_MAX_OUTPUTS];

    for(u64 i = 0; i < nb_layers; i++){
        layers[i] = createLayer( neurons_per_layers[i], neurons_per_layers[i+1]);
        // printf("%lld, %x\n", i, layers[i] );
    }


    for(u64 i = 0; i < nb_layers - 1; i++){
        initLayer( layers[i], neurons_per_layers[i+1] );
    }

    // for each images
    for (size_t img_iter = 1 ; img_iter < argc ; img_iter++) {

        printf("\nImage %s :\n", argv[img_iter]);

        int * input = process_img(argv[img_iter]);

        u64 train_id = 0;
        while( train_id < 4000000){
            
            train_id ++;
            int a = rand() % 10 ; // random betwenn 0 and 10 
            int b = rand() % 10 ; // random betwenn 0 and 10 
            int c = 0.0;
            f64 err = 0.0; 

            
            // setting inputs and expected output
            if( ( (f64) rand() / (f64) RAND_MAX )  > 0.5 ){
                c = a + b;
                expected[0] = 1.0;
                
            }
            else{
                do {
                    c = rand() % 19 ; // random betwenn 0 and 10 
                    }
                while(c == a + b);
                expected[0] = 0.0;
            }
            
            
            //reduce each numbers to a value between -4 and 4, and use activation function on it.
            layers[0]->neurons[0] = sigmoid( ((f64) a - 4.5) / 2 );
            layers[0]->neurons[1] = sigmoid( ((f64) b - 4.5) / 2 );
            layers[0]->neurons[2] = sigmoid( ((f64) c - 9) / 4 );

            
            // compute
            for(u64 i = 0; i < nb_layers - 1; i++){
                computeLayer( layers[i], layers[i+1] );
            }
            
            //delta
            err = computeOutputDelta( layers[nb_layers - 1] , expected );

            cumul_err += err;


            //error
            if( train_id % 1000 == 0){
            // if( 1 == 1){
                //printf(" %llu; %lf\n", train_id, cumul_err/1000);
                cumul_err = 0;
                // getchar();
            }

            for(u64 i = nb_layers - 2; i > 0; i--){
                // printf("%lld %lld\n", i, i+1);
                computeDelta( layers[i], layers[i+1] );
            }

            for(u64 i = 0; i < nb_layers - 1; i++){
                backpropagate( layers[i], layers[i+1] );
            }
        


            continue;

            printf("####################################\n");
            for(u64 i = 0; i < nb_layers; i++){
                u64 nextsize;
                if( i < nb_layers - 1){
                    nextsize = layers[i+1]->size;
                }
                else{
                    nextsize = 0;
                }
                // printf("i: %lld\n", i);
                // debug( layers[i], nextsize );
            }

            getchar();

        }
    } 
}


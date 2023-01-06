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
#include "store.h"  

#define NB_MAX_LAYER   50
#define NB_MAX_OUTPUTS 50

int main(int argc, char *argv[])
{

    
    srand(time(NULL));

    char * dirs[] = { "dataset/train/NonDemented", "dataset/train/VeryMildDemented"};
    
    
    
    int num_folder = 2;
    int max_per_folder = 20;

    u64 nb_layers = 5;
    u64 neurons_per_layers[NB_MAX_LAYER] = {480,200,50,10,1,1};
    u64 input_size = neurons_per_layers[0];
    f64 expected[NB_MAX_OUTPUTS];
    u64 train_max = 20;

    
    // number of images
    for( int i = 0; i < 2; i++ ){
        int num = count_file(dirs[i]);
        printf(" file in %s :  %d\n", dirs[i],num);
    }

    mri_image * dataset  = malloc( num_folder * max_per_folder * sizeof(mri_image) );
    u64 * random_pattern = malloc( num_folder * max_per_folder * sizeof(u64));
    
    // fill dataset
    u64 counter = (u64) load_dataset( dirs, num_folder, dataset, max_per_folder);

    printf("dataset filled : %llu\n", counter);
    
    
    
    //  Initialise The NN
    Layer ** layers = Init_Neural_network(neurons_per_layers, nb_layers);
    
    printf(" id ; err\n");

    //  Training
    for( u64 train_id = 0; train_id < train_max; train_id++ ){
        f64 cumul_err = 0.0f;
        f64 err = 0.0f; 
        //randomize dataset
        shuffle(counter, random_pattern);
      
        // 
        for( u64 np = 0 ; np < counter ; np++ ) {
            u64 p = random_pattern[np];
            
            //set input
            fill_input(layers[0], input_size, dataset[p].inputs);
            
            // set expected
            expected[0] = dataset[p].value;

            // compute
            forward_compute( nb_layers, layers );

            //error
            err = get_error( layers[nb_layers - 1] , expected );
            cumul_err += err;
            
            backward_compute(nb_layers, layers, expected );
        }
        
        //error
        printf(" %llu; %lf\n", train_id, cumul_err/counter);
        cumul_err = 0;
    }
    store_nn("storage", layers, nb_layers, neurons_per_layers);
    // load_nn("storage", nb_layers, neurons_per_layers);

    free_all(layers, nb_layers);

    for (size_t i = 0; i < counter; i++) {
        free(dataset[i].inputs);
    }
    
    free(dataset);
    free(random_pattern);
}

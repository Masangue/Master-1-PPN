#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "type.h"
#include "neural_network.h"
#include "convolution_layer.h"
#include "dataset_manager.h"
#include "store.h"  
#include "context.h"  
#include "evaluation.h" 
#include "training.h" 

#define NB_MAX_LAYER   50
#define NB_MAX_OUTPUTS 50


/*  NN-training function, calling previously defined functions 
    The function starts by loading the "train" dataset and 
    proccessing all its files.

    It then feeds them to the NN one by one, recording the error produced at the output
    and backpropagating it
    (this part is repeated train_max times)
*/
int main(int argc, char *argv[])
{

    Context context;
    load_context( &context, "../data/config.cfg" );
    info_context( &context );


    srand(time(NULL));
    
    // char * train_dirs[] = { "../dataset/train/NonDemented", "../dataset/train/ModerateDemented"};
    // char * test_dirs[]  = { "../dataset/test/NonDemented" , "../dataset/test/ModerateDemented"};
    
    FILE * fpTrain = fopen( context.train_dat_path,"w"); 
    FILE * fpTest = fopen( context.test_dat_path,"w"); 
    
    int num_folder = 2;
    int max_per_folder = context.max_per_folder;

    // u64 nb_layers = 5;
    // u64 neurons_per_layers[NB_MAX_LAYER] = {480,200,50,10,1,1};
    
    // u64 nb_layers = 7;
    // u64 neurons_per_layers[NB_MAX_LAYER] = {480,64,64,64,64,64,1,1};
    // u64 epoch_max = 100;

    
    // number of images
    // for( int i = 0; i < 2; i++ ){
    //     int num = count_file(trainDirs[i]);
    //     printf(" file in %s :  %d\n", trainDirs[i],num);
    // }

    Dataset train_dataset;
    Dataset test_dataset;

    // TODO la taille du dataset est mise au max theorique d'element et non au nombre reel d'element
    train_dataset.size = num_folder * max_per_folder;  
    test_dataset.size  = num_folder * max_per_folder; 
    train_dataset.images = malloc( train_dataset.size * sizeof(mri_image) );
    test_dataset.images  = malloc( test_dataset.size  * sizeof(mri_image) );

    // u64 * random_pattern = malloc( num_folder * max_per_folder * sizeof(u64));
    
    // fill dataset
    u64 train_dataset_size = (u64) load_dataset( context.train_dirs, num_folder, &train_dataset, max_per_folder);
    u64 test_dataset_size  = (u64) load_dataset( context.test_dirs, num_folder, &test_dataset, max_per_folder);

    printf("Dataset filled with %llu images\n", train_dataset_size);
    printf("Test dataset filled with %llu images\n", test_dataset_size);
    
                    
    
    //  Initialise The NN
    Layer ** neural_network = init_neural_network(context.topology, context.nn_size);

   
    train( &context, &train_dataset, &test_dataset, neural_network, fpTest, fpTrain );

    fclose(fpTrain);
    fclose(fpTest);
    store_neural_network( &context, neural_network);
    
    free_neural_network( neural_network, context.nn_size );
    free_dataset( &train_dataset ); 
    free_dataset( &test_dataset ); 
    free_context( &context ); 

}

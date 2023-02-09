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
#include "evaluation.h" 

#define NB_MAX_LAYER   50
#define NB_MAX_OUTPUTS 50

// print an ascii art of a 20*24 images
// this function will be remove or move
void print_input(u8 * inputs){
    for(int h = 0; h < 24; h++){
        for(int w = 0; w < 20; w++){
            int val = inputs[h * 20 + w];
            char s;
            if( val < 20)
                s = ' ';
            else if ( val < 50)
                s = '.';
            else if ( val < 100)
                s = ':';
            else if ( val < 150)
                s = 'o';
            else 
                s = '%';

            printf("%c%c%c",s,s,s);
        }
    printf("\n"); 
    }
    printf("\n\n"); 
}

/*  NN-training function, calling previously defined functions 
    The function starts by loading the "train" dataset and 
    proccessing all its files.

    It then feeds them to the NN one by one, recording the error produced at the output
    and backpropagating it
    (this part is repeated train_max times)
*/
int main(int argc, char *argv[])
{

    
    srand(time(NULL));

    
    char * trainDirs[] = { "../dataset/train/NonDemented", "../dataset/train/ModerateDemented"};
    char * testDirs[]  = { "../dataset/test/NonDemented" , "../dataset/test/ModerateDemented"};
    
    FILE * fpTrain = fopen("../out/data/train.dat","w"); 
    FILE * fpTest = fopen("../out/data/test.dat","w"); 
    
    int num_folder = 2;
    int max_per_folder = 2000;

    // u64 nb_layers = 5;
    // u64 neurons_per_layers[NB_MAX_LAYER] = {480,200,50,10,1,1};
    
    u64 nb_layers = 7;
    u64 neurons_per_layers[NB_MAX_LAYER] = {480,64,64,64,64,64,1,1};
    u64 input_size = neurons_per_layers[0];
    f64 expected[NB_MAX_OUTPUTS];
    u64 train_max = 100;

    
    // number of images
    // for( int i = 0; i < 2; i++ ){
    //     int num = count_file(trainDirs[i]);
    //     printf(" file in %s :  %d\n", trainDirs[i],num);
    // }

    mri_image * dataset  = malloc( num_folder * max_per_folder * sizeof(mri_image) );
    mri_image * testDataset  = malloc( num_folder * max_per_folder * sizeof(mri_image) );
    u64 * random_pattern = malloc( num_folder * max_per_folder * sizeof(u64));
    
    // fill dataset
    u64 train_dataset_size = (u64) load_dataset( trainDirs, num_folder, dataset, max_per_folder);
    u64 test_dataset_size  = (u64) load_dataset( testDirs, num_folder, testDataset, max_per_folder);

    printf("Dataset filled with %llu images\n", train_dataset_size);
    printf("Test dataset filled with %llu images\n", test_dataset_size);
    
                    
    
    //  Initialise The NN
    Layer ** layers = Init_Neural_network(neurons_per_layers, nb_layers);
    Score score;

    printf(" epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    fprintf(fpTest," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    fprintf(fpTrain," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");

    //  Training
    for( u64 epoch = 0; epoch < train_max; epoch++ ){
        initScore(&score);
        //randomize dataset
        shuffle(train_dataset_size, random_pattern);
        
        
        // TRAIN 
        for( u64 np = 0 ; np < train_dataset_size ; np++ ) {
            u64 p = random_pattern[np];
                        
            // print_input( dataset[p].inputs);

            fill_input( layers[0], input_size, dataset[p].inputs );
            expected[0] = dataset[p].value;
            forward_compute( nb_layers, layers );
            updateScore(  layers[nb_layers - 1], expected, &score );
            backward_compute(nb_layers, layers, expected );

        }


        processScore( &score );
        fprintf(fpTrain, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1,  score.specificity );
        printf( "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1,  score.specificity );
    
        // TEST
        initScore(&score);
        for( u64 np = 0 ; np < test_dataset_size ; np++ ) {
            u64 p = np;

            fill_input( layers[0], input_size, testDataset[p].inputs );
            expected[0] = testDataset[p].value;
            forward_compute( nb_layers, layers );
            updateScore(  layers[nb_layers - 1], expected, &score );
        }
        processScore( &score );
        fprintf(fpTest, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1, score.specificity );
    

    }

    fclose(fpTrain);
    fclose(fpTest);

    store_nn("../out/storage", layers, nb_layers, neurons_per_layers);

    // free all and quit
    free_all(layers, nb_layers);

    for (size_t i = 0; i < train_dataset_size; i++)
        free( dataset[i].inputs );
    
    for (size_t i = 0; i < test_dataset_size; i++)
        free( testDataset[i].inputs );
    
    free(dataset);
    free(testDataset);
    free(random_pattern);
}

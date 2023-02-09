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

    char * dirs[] = { "../dataset/train/NonDemented", "../dataset/train/ModerateDemented"};
    
    
    
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
                        
            // print_input( dataset[p].inputs);

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
    store_nn("../out/storage", layers, nb_layers, neurons_per_layers);


    // free all and quit
    free_all(layers, nb_layers);

    for (size_t i = 0; i < counter; i++) {
        free(dataset[i].inputs);
    }
    
    free(dataset);
    free(random_pattern);
}

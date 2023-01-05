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
#include <dirent.h>
#include <string.h>


#include "type.h"
#include "nn.h"
#include "image_processing.h"

#define NB_MAX_LAYER   50
#define NB_MAX_OUTPUTS 50

typedef struct {
    char * filename;
    int value ;
    unsigned char * inputs;
} mri_image;

int main(int argc, char *argv[])
{

    // if(argc < 2)
    // {
    //     printf("no input file\n");
    //     return 1;
    // }
    // 
    srand(time(NULL));

    char * dirs[] = { "dataset/Axial/AD", "dataset/Axial/CN"};
    int counter = 0; 

    // number of images
    for( int i = 0; i < 2; i++ ){
        DIR *d;
        struct dirent *dir;
        d = opendir(dirs[i]);
        printf("%s\n", dirs[i]);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
                    continue;
                }
                counter++;
                // char * filename = dir->d_name;
                // printf("    %s\n", dir->d_name);
            }
            closedir(d);
        }
    }

    // fill dataset
    mri_image * dataset = malloc( counter * sizeof(mri_image) );
    u64 * random_pattern = malloc( counter );
   
    u64 n = 0;
    for( int i = 0; i < 2; i++ ){
        DIR *d;
        struct dirent *dir;
        d = opendir(dirs[i]);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
                    continue;
                }
                char buf[256];
                snprintf(buf, sizeof(buf), "%s/%s", dirs[i], dir->d_name);
                
                dataset[n].inputs = (unsigned char *) prepare_image(buf);
                dataset[n].value = i;
                dataset[n].filename = malloc( 256 * sizeof(char));
                strcpy(dataset[n].filename, buf);

                // char * filename = dir->d_name;
                printf("%s\n", dir->d_name);
                n++;
            }
            closedir(d);
        }
    }

    printf("dataset filled : %llu\n", n);

    
    


    // int * input = process_img(argv[img_iter]);


    
    //  Initialise The NN
    f64 cumul_err = 0.0f;
    f64 err = 0.0; 

    u64 nb_layers = 4;
    u64 neurons_per_layers[NB_MAX_LAYER] = {10,8,4,1,1};
    Layer ** layers = malloc( nb_layers * sizeof(Layer *) );

    f64 expected[NB_MAX_OUTPUTS];

    for(u64 i = 0; i < nb_layers; i++){
        layers[i] = createLayer( neurons_per_layers[i], neurons_per_layers[i+1]);
        // printf("%lld, %x\n", i, layers[i] );
    }

    for(u64 i = 0; i < nb_layers - 1; i++){
        initLayer( layers[i], neurons_per_layers[i+1] );
    }

    printf(" id ; err\n");

    //  Trainsing
    for( u64 train_id = 0; train_id < 1000; train_id++ ){
        
        //randomize dataset
        for( u64 p = 0 ; p < counter ; p++ ) {    /* randomize order of training patterns */
            random_pattern[p] = p ;
        }

        for( u64 p = 0 ; p < counter - 1 ; p++) {
            u64 np = rand() % ( counter - p) + p;
            // np = p + rando() * ( NumPattern + 1 - p ) ;

            u64 op = random_pattern[p] ;
            random_pattern[p] = random_pattern[np];
            random_pattern[np] = op;
        }


        for( u64 np = 0 ; np < counter ; np++ ) {
            u64 p = random_pattern[np];
            // printf("%lld  :  %lld / %d  %d", train_id, np, counter, dataset[p].value);


            for( u64 i = 0; i < 10; i++ ){
                layers[0]->neurons[i] = sigmoid( dataset[p].inputs[i] );
//             layers[0]->neurons[0] = sigmoid( ((f64) a - 4.5) / 2 );
            }

            expected[0] = dataset[p].value;

            // compute
            for(u64 i = 0; i < nb_layers - 1; i++){
                computeLayer( layers[i], layers[i+1] );
            }
            
            //delta
            err = computeOutputDelta( layers[nb_layers - 1] , expected );

            cumul_err += err;
            // printf("err %f \n", err);


            

            for(u64 i = nb_layers - 2; i > 0; i--){
                // printf("%lld %lld\n", i, i+1);
                computeDelta( layers[i], layers[i+1] );
            }

            for(u64 i = 0; i < nb_layers - 1; i++){
                backpropagate( layers[i], layers[i+1] );
            }
        }
//error
            // if( train_id % 1000 == 0){
            // if( 1 == 1)
        printf(" %llu; %lf\n", train_id, cumul_err/counter);
        cumul_err = 0;
                // getchar();
            // }
    }
}

    // for each images
    // for (size_t img_iter = 1 ; img_iter < argc ; img_iter++) {
    //
    //     printf("\nImage %s :\n", argv[img_iter]);
    //
    //     // int * input = process_img(argv[img_iter]);
    //
    //     u64 train_id = 0;
    //     while( train_id < 4000000){
    //         
    //         train_id ++;
    //         int a = rand() % 10 ; // random betwenn 0 and 10 
    //         int b = rand() % 10 ; // random betwenn 0 and 10 
    //         int c = 0.0;
        // f64 err = 0.0; 
    //
    //         
    //         // setting inputs and expected output
    //         if( ( (f64) rand() / (f64) RAND_MAX )  > 0.5 ){
    //             c = a + b;
    //             expected[0] = 1.0;
    //             
    //         }
    //         else{
    //             do {
    //                 c = rand() % 19 ; // random betwenn 0 and 10 
    //                 }
    //             while(c == a + b);
    //             expected[0] = 0.0;
    //         }
    //         
    //         
    //         //reduce each numbers to a value between -4 and 4, and use activation function on it.
//             layers[0]->neurons[0] = sigmoid( ((f64) a - 4.5) / 2 );
//             layers[0]->neurons[1] = sigmoid( ((f64) b - 4.5) / 2 );
//             layers[0]->neurons[2] = sigmoid( ((f64) c - 9) / 4 );
//
//             
//             // compute
//             for(u64 i = 0; i < nb_layers - 1; i++){
//                 computeLayer( layers[i], layers[i+1] );
//             }
//             
//             //delta
//             err = computeOutputDelta( layers[nb_layers - 1] , expected );
//
//             cumul_err += err;
//
//
//             //error
//             if( train_id % 1000 == 0){
//             // if( 1 == 1){
//                 //printf(" %llu; %lf\n", train_id, cumul_err/1000);
//                 cumul_err = 0;
//                 // getchar();
//             }
//
//             for(u64 i = nb_layers - 2; i > 0; i--){
//                 // printf("%lld %lld\n", i, i+1);
//                 computeDelta( layers[i], layers[i+1] );
//             }
//
//             for(u64 i = 0; i < nb_layers - 1; i++){
//                 backpropagate( layers[i], layers[i+1] );
//             }
//         
//
//
//             continue;
//
//             printf("####################################\n");
//             for(u64 i = 0; i < nb_layers; i++){
//                 u64 nextsize;
//                 if( i < nb_layers - 1){
//                     nextsize = layers[i+1]->size;
//                 }
//                 else{
//                     nextsize = 0;
//                 }
//                 // printf("i: %lld\n", i);
//                 // debug( layers[i], nextsize );
//             }
//
//             getchar();
//
//         }
//     } 
// }

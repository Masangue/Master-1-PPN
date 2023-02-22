#include "training.h"
#include <stdio.h>

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


int train(u64 max_epoch, f64 precision, Dataset * train_dataset, 
          Dataset * test_dataset, Layer ** neural_network, 
          u64 nb_layers, FILE * fp_train, FILE * fp_test ) // TODO cette ligne doit etre suprimee
        {
   
    printf(" epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    fprintf(fp_test," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    fprintf(fp_train," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
  
    Score score;
    
    u64 nn_size = nb_layers;
    u64 input_size = neural_network[0]->size;
    u64 output_size = neural_network[nn_size-1]->size;

    f64 expected[output_size];
    u64 * random_pattern = malloc( train_dataset->size * sizeof(u64));


    u8 *image_ptr = NULL;
    u8 *buffer_ptr = NULL;

    image_ptr  = malloc( IMAGE_SIZE * sizeof(unsigned char));
    buffer_ptr = malloc( IMAGE_SIZE * sizeof(unsigned char));


    for( u64 i  = 0; i < train_dataset->size; i++ ){

        memcpy(image_ptr, train_dataset->images[i].pixels, sizeof(u8) *  
               train_dataset->images[i].width * train_dataset->images[i].height);

        train_dataset->images[i].inputs = apply_convolution_filters(
                image_ptr, buffer_ptr, 
                train_dataset->images[i].width, 
                train_dataset->images[i].height );
    }

    for( u64 i  = 0; i < test_dataset->size; i++ ){

        memcpy(image_ptr, test_dataset->images[i].pixels, sizeof(u8) *  
               test_dataset->images[i].width * test_dataset->images[i].height);

        test_dataset->images[i].inputs = apply_convolution_filters(
                image_ptr, buffer_ptr, 
                test_dataset->images[i].width, 
                test_dataset->images[i].height );
    }
    
    free(image_ptr);
    free(buffer_ptr);


    
    for( u64 epoch = 0; epoch < max_epoch; epoch++ ){
        initScore(&score);
        shuffle(train_dataset->size, random_pattern);
         
        
        for( u64 np = 0 ; np < train_dataset->size ; np++ ) {
            u64 p = random_pattern[np];
            // print_input( train_dataset->images[p].inputs );

            fill_input( neural_network[0], input_size, train_dataset->images[p].inputs );
            expected[0] = train_dataset->images[p].value;
            forward_compute( nb_layers, neural_network );
            updateScore(  neural_network[nn_size - 1], expected, &score );
            backward_compute(nb_layers, neural_network, expected );

        }


        processScore( &score );
        fprintf(fp_train, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1,  score.specificity );
        printf( "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1,  score.specificity );
    
        // TEST
        initScore(&score);
        for( u64 p = 0 ; p < test_dataset->size ; p++ ) {

            fill_input( neural_network[0], input_size, test_dataset->images[p].inputs );
            expected[0] = test_dataset->images[p].value;
            forward_compute( nb_layers, neural_network );
            updateScore(  neural_network[nb_layers - 1], expected, &score );
        }
        processScore( &score );
        fprintf(fp_test, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1, score.specificity );
    

    }

    free(random_pattern);

    return 1;
}

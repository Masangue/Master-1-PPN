#include "training.h"



int train(Context * context, Dataset * train_dataset, 
          Dataset * test_dataset, Layer ** neural_network, 
          FILE * fp_train, FILE * fp_test ) // TODO cette ligne doit etre suprimee
        {
   
    printf(" epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    fprintf(fp_test," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    fprintf(fp_train," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
  
    Score score;
    
    u64 nn_size = context->nn_size;
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


    
    for( u64 epoch = 0; epoch < context->max_epoch; epoch++ ){
        init_score(&score);
        shuffle(train_dataset->size, random_pattern);
         
        
        for( u64 np = 0 ; np < train_dataset->size ; np++ ) {
            u64 p = random_pattern[np];
            // display_ascii_image( train_dataset->images[p].inputs, train_dataset->images[p].width, train_dataset->images[p].height );

            fill_input( neural_network[0], input_size, train_dataset->images[p].inputs );
            expected[0] = train_dataset->images[p].value;
            forward_compute( nn_size, neural_network, context );
            update_score(  neural_network[nn_size - 1], expected, &score );
            backward_compute( neural_network, expected, context );

        }


        process_score( &score );
        fprintf(fp_train, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1,  score.specificity );
        printf( "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1,  score.specificity );
    
        // TEST
        init_score(&score);
        for( u64 p = 0 ; p < test_dataset->size ; p++ ) {

            fill_input( neural_network[0], input_size, test_dataset->images[p].inputs );
            expected[0] = test_dataset->images[p].value;
            forward_compute( nn_size, neural_network, context );
            update_score(  neural_network[nn_size - 1], expected, &score );
        }
        process_score( &score );
        fprintf(fp_test, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score.precision, score.recall, score.accuracy, score.f1, score.specificity );
    

    }

    free(random_pattern);

    return 1;
}

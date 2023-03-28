#include "training.h"
#include "neural_network.h"
#include <stdio.h>


void log_score(FILE * fp, u64 epoch, Score * score){
    fprintf(fp, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
    printf( "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
}

f64 stochastic_gradient_descent( Dataset * dataset, Neural_network * neural_network, Context * context, Score * score, u64 * scheduler, FILE * fp, u64 epoch, u64 is_learning ){
 
    u64 nn_size = neural_network->size;
    // u64 output_size = neural_network->layers[nn_size-1].size;
    
    init_score(score);
    for( u64 np = 0 ; np < dataset->size ; np++ ) {
        u64 p = scheduler[np];

        set_input_output(neural_network, dataset->images[p].inputs, &dataset->images[p].value, 0 );
        
        stochastic_forward_compute( neural_network, context );
        update_score(  &neural_network->layers[nn_size - 1], 
                     neural_network->expected, score, 0 );

        if( is_learning){
            stochastic_backward_compute( neural_network, context );
        }
    }

    process_score( score );
    log_score(fp, epoch, score);
    return score->f1;
}



f64 one_batch_train( Dataset * dataset, Neural_network * neural_network, Context * context,
                 u64 batch_size, u64 * scheduler, Score * score ){

   
    for( u64 j = 0 ; j < batch_size ; j++ ) {
        u64 p = scheduler[ j ];
        u64 bi = j;

        //
        // update_batch_pointer( neural_network, bi);

        // input and expected
        set_input_output(neural_network, dataset->images[p].inputs,
                         &dataset->images[p].value, bi );

        //
        batch_forward_propagation(neural_network, context, bi);
        update_score( &neural_network->layers[neural_network->size - 1], 
                     neural_network->expected, score, bi );
    }
    
    batch_backward_propagation( neural_network, context, batch_size );

    return 0;
}



f64 batch_gradient_descent( Dataset * dataset, Neural_network * neural_network, Context * context, 
                           Score * score, u64 * scheduler, FILE * fp, u64 epoch ){

    u64 batch_size = context->batch_size;
    u64 dataset_size = dataset->size;

    init_score(score);
   
    u64 number_of_complete_batch = dataset_size / batch_size;
    for( u64 i = 0 ; i < number_of_complete_batch ; i++ ) {
        one_batch_train(dataset, neural_network, context, batch_size, 
                        scheduler + batch_size * i, score);
    }

    if( dataset_size % batch_size > 0)
        one_batch_train(dataset, neural_network, context, dataset_size % batch_size,
                        scheduler + number_of_complete_batch * batch_size, score);

    process_score( score );
    log_score(fp, epoch, score);
    return score->f1;
}


int train(Context * context, Dataset * train_dataset, 
          Dataset * test_dataset, Neural_network * neural_network  )
        {
   
    FILE * fp_train;
    FILE * fp_test ;
    fp_train = fopen( context->train_dat_path,"w"); 
    fp_test = fopen( context->test_dat_path,"w"); 

 
    Score score;
    
    u64 * train_scheduler = malloc( train_dataset->size * sizeof(u64));
    u64 * test_scheduler = malloc( test_dataset->size * sizeof(u64));
    range(test_dataset->size, test_scheduler);
    range(test_dataset->size, test_scheduler);

    preprocess_dataset( train_dataset, context );
    preprocess_dataset( test_dataset, context );


    float training_score = 0; 
    for( u64 epoch = 0; epoch < (u64) context->max_epoch && training_score + context->precision < 1; epoch++ ){
        shuffle(train_dataset->size, train_scheduler);
        
        if (context->batch_size > 1)
            training_score = batch_gradient_descent( train_dataset, neural_network, context, &score, train_scheduler, fp_train, epoch );
        else
            training_score = stochastic_gradient_descent( train_dataset, neural_network, context, &score, train_scheduler, fp_train, epoch, 1 );

        stochastic_gradient_descent( test_dataset,  neural_network, context, &score, test_scheduler,  fp_test,  epoch, 0 );
        
        }

    free(test_scheduler);
    free(train_scheduler);

    fclose(fp_train);
    fclose(fp_test);

    return 1;
}

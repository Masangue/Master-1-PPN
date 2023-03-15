#include "training.h"
#include "neural_network.h"
#include <stdio.h>


void log_score(FILE * fp, u64 epoch, Score * score){
    fprintf(fp, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
    printf( "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
}

f64 train_one_epoch( Dataset * dataset, Neural_network * neural_network, Context * context, Score * score, u64 * scheduler, FILE * fp, u64 epoch, u64 is_learning ){
 
    u64 nn_size = neural_network->size;
    u64 input_size = neural_network->layers[0].size;
    // u64 output_size = neural_network->layers[nn_size-1].size;
    
    init_score(score);
    for( u64 np = 0 ; np < dataset->size ; np++ ) {
       u64 p = scheduler[np];

       fill_input( &neural_network->layers[0], input_size, dataset->images[p].inputs );
       neural_network->expected[0] = dataset->images[p].value;
       forward_compute( neural_network, context );
       update_score(  &neural_network->layers[nn_size - 1], neural_network->expected, score );
       if( is_learning){
           backward_compute( neural_network, context );
       }
    }

    process_score( score );
    log_score(fp, epoch, score);
    return score->f1;
}



f64 one_batch_train( Dataset * dataset, Neural_network * neural_network, Context * context,
                 u64 current_batch_size, u64 batch_number, u64 * scheduler, Score * score ){

    u64 nn_size = neural_network->size;
    u64 input_size = neural_network->layers[0].size;   
    // u64 batch_size = context->batch_size;

    // printf("\n");
    // printf("# %llu \n", current_batch_size );


    // current_batch_file is inferior or equals batch_size
    for( u64 j = 0 ; j < current_batch_size ; j++ ) {
        // printf("elem %llu \n batch number : %llu \n batch size %llu \n j : %llu\n", batch_number * batch_size + j, batch_number, current_batch_size, j);
        // u64 p = scheduler[batch_number * batch_size + j];
        u64 p = scheduler[ j ];

        //
        update_batch_pointer( neural_network, j);

        // in out
        fill_input( &neural_network->layers[0], input_size, dataset->images[p].inputs );
        neural_network->expected[0] = dataset->images[p].value;
        batch_forward_propagation(neural_network, context);
        // printf("%llu   out1 : %f \n", j, neural_network->layers[nn_size-1].neurons[0] );

        //
        update_score( &neural_network->layers[nn_size - 1], neural_network->expected, score );
    }


    // for( u64 j = 0 ; j < current_batch_size ; j++ ) {
    //     printf("%llu   out2 : %f \n", j, neural_network->layers[nn_size-1].batch_neurons[j] );
    // }
    
    batch_backward_propagation( neural_network, context, current_batch_size );

    return 0;
}

f64 batch_train( Dataset * dataset, Neural_network * neural_network, Context * context, Score * score, u64 * scheduler, FILE * fp, u64 epoch, u64 is_learning ){
 

    // u64 nn_size = neural_network->size;
    // u64 output_size = neural_network->layers[nn_size-1].size;
    u64 batch_size = context->batch_size;

    u64 dataset_size = dataset->size;

    init_score(score);
    
    for( u64 i = 0 ; i < dataset_size / batch_size ; i++ ) {
        one_batch_train(dataset, neural_network, context, batch_size, i, 
                     scheduler + batch_size * i, score);
    }

    u64 batch_number = dataset_size / batch_size;
    if( dataset_size % batch_size > 0)
        one_batch_train(dataset, neural_network, context, dataset_size % batch_size,
                 batch_number, scheduler + batch_number * batch_size, score);
    

    // printf("\n");
    // printf("\n");
    // printf("\n");
    

    update_batch_pointer( neural_network, 0);

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


    // printf(" epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    // fprintf(fp_test," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");
    // fprintf(fp_train," epoch; precision; recall; accuracy; f1; falsePositiveRate \n");

    preprocess_dataset( train_dataset, context );
    preprocess_dataset( test_dataset, context );


    float training_score = 0; 
    for( u64 epoch = 0; epoch < context->max_epoch && training_score + context->precision < 1; epoch++ ){
        shuffle(train_dataset->size, train_scheduler);
         
        // training_score = train_one_epoch( train_dataset, neural_network, context, &score, train_scheduler, fp_train, epoch, 1 );
        training_score = batch_train( train_dataset, neural_network, context, &score, train_scheduler, fp_train, epoch, 1 );
        train_one_epoch( test_dataset,  neural_network, context, &score, test_scheduler,  fp_test,  epoch, 0 );
        
        }

    free(test_scheduler);
    free(train_scheduler);

    fclose(fp_train);
    fclose(fp_test);

    return 1;
}

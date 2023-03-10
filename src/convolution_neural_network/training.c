#include "training.h"


int train_one_epoch( Dataset * dataset, Neural_network * neural_network, Context * context, Score * score,u64 * scheduler, FILE * fp, u64 epoch, u64 is_learning ){
 
    u64 nn_size = neural_network->size;
    u64 input_size = neural_network->layers[0].size;
    // u64 output_size = neural_network->layers[nn_size-1].size;


    init_score(score);
    for( u64 np = 0 ; np < dataset->size ; np++ ) {
       u64 p = scheduler[np];
       // display_ascii_image( train_dataset->images[p].inputs, train_dataset->images[p].width, train_dataset->images[p].height );

       fill_input( &neural_network->layers[0], input_size, dataset->images[p].inputs );
       neural_network->expected[0] = dataset->images[p].value;
       forward_compute( neural_network, context );
       update_score(  &neural_network->layers[nn_size - 1], neural_network->expected, score );
       if( is_learning){
           backward_compute( neural_network, context );
                    }

    }

    process_score( score );


    fprintf(fp, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
    printf( "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
    

    return 0;
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


    
    for( u64 epoch = 0; epoch < context->max_epoch; epoch++ ){
        shuffle(train_dataset->size, train_scheduler);
         
        train_one_epoch( train_dataset, neural_network, context, &score, train_scheduler, fp_train, epoch, 1 );
        train_one_epoch( test_dataset,  neural_network, context, &score, test_scheduler,  fp_test,  epoch, 0 );
       
        }

    free(test_scheduler);
    free(train_scheduler);

    fclose(fp_train);
    fclose(fp_test);

    return 1;
}

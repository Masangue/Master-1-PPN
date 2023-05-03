#include "training.h"
#include "layer.h"
#include "neural_network.h"
#include <stdio.h>
#include <unistd.h>


void log_score(FILE * fp, u64 epoch, Score * score){
    fprintf(fp, "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
    printf( "%llu; %lf; %lf; %lf; %lf; %lf\n", epoch, score->precision, score->recall, score->accuracy, score->f1,  score->specificity );
}


f64 stochastic_gradient_descent( Dataset * dataset, Neural_network * neural_network, Context * context, Score * score, u64 * scheduler, FILE * fp, u64 epoch, u64 is_learning ){
 
    u64 nn_size = neural_network->size;
    
    init_score(score);
    for( u64 np = 0 ; np < dataset->size ; np++ ) {
        u64 p = scheduler[np];

        set_input_output(neural_network, dataset->images[p].inputs, &dataset->images[p].value );
        
        stochastic_forward_compute( neural_network, context );
        update_score(  &neural_network->layers[nn_size - 1], 
                     neural_network->expected, score );

        if( is_learning){
            stochastic_backward_compute( neural_network, context );
        }
    }

    process_score( score );
    log_score(fp, epoch, score);
    return score->f1;
}



f64 one_batch_train( Dataset * dataset, Neural_network * neural_network, 
                    Context * context, Mpi_neural_network_context * mpi_nn_context,
                    u64 batch_size, u64 * scheduler, Score * score ){

    int rank, P;
    rank = mpi_nn_context->rank;
    P    = mpi_nn_context->P;

    u64 workload = mpi_nn_context->workload[rank];

    #pragma omp parallel
    {

        for( u64 j = 0 ; j < workload ; j++ ) {
            u64 global_batch_iteration = rank + j * P ; // global batch iteration
            u64 local_batch_iteration = j ; // global batch iteration
            u64 p = scheduler[ global_batch_iteration ];

            // input and expected
            set_input_output(neural_network, dataset->images[p].inputs,
                            &dataset->images[p].value );

            batch_forward_propagation(neural_network, context, local_batch_iteration);
        
            #pragma omp single
            {
                update_score( &neural_network->layers[neural_network->size - 1], 
                        neural_network->expected, score );
            }
        }

    }

    mpi_reduce_gradient( neural_network, mpi_nn_context );
  
    // if( rank == ROOT )

    #pragma omp parallel
    {
        batch_backward_propagation( neural_network, context, batch_size );
    }

    // mpi_share_neural_network(neural_network);

    return 0;
}



f64 batch_gradient_descent( Dataset * dataset, Neural_network * neural_network, 
                           Context * context, Mpi_neural_network_context * mpi_nn_context, 
                           Score * score, u64 * scheduler, FILE * fp, u64 epoch ){

    u64 batch_size = context->batch_size;
    u64 dataset_size = dataset->size;

    init_score(score);
   
    u64 number_of_complete_batch = dataset_size / batch_size;
    for( u64 i = 0 ; i < number_of_complete_batch ; i++ ) {
        // printf("BATCH NUM %llu \n", i);
        one_batch_train(dataset, neural_network, context, mpi_nn_context, 
                        batch_size, scheduler + batch_size * i, score);
    }

    // debug( &neural_network->layers[3] );
    // getchar();

    // we suppose for now that this part is not significant
    // if( dataset_size % batch_size > 0)
    //     one_batch_train(dataset, neural_network, context, dataset_size % batch_size,
    //                     scheduler + number_of_complete_batch * batch_size, score);
    
    mpi_sync_score( score, mpi_nn_context); 
    if (mpi_nn_context->rank == MASTER_RANK) {
        process_score( score );
        log_score(fp, epoch, score);
    }
    return score->f1;
}



int train(Context * context, Dataset * train_dataset, 
          Dataset * test_dataset, Neural_network * neural_network  )
        {
   
    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    Mpi_neural_network_context mpi_nn_context = 
        create_mpi_neural_network_context(neural_network, context->batch_size);

    
    

    FILE * fp_train = NULL;
    FILE * fp_test = NULL;
    if( rank == ROOT ){
        fp_train = fopen( context->train_dat_path,"w"); 
        fp_test = fopen( context->test_dat_path,"w"); 
    }


    Score score;

    u64 * train_scheduler = malloc( train_dataset->size * sizeof(u64));
    u64 * test_scheduler = malloc( test_dataset->size * sizeof(u64));
    range(test_dataset->size,  test_scheduler);
    range(train_dataset->size, train_scheduler);

    preprocess_dataset( train_dataset, context );
    preprocess_dataset( test_dataset, context );


    float training_score = 0;
    for( u64 epoch = 0; epoch < (u64) context->max_epoch /*&& training_score + context->precision < 1 */; epoch++ ){
        context->eta_ = context->eta_ * 0.97;
        shuffle(train_dataset->size, train_scheduler);

        // if (context->batch_size > 1)
        training_score = batch_gradient_descent( train_dataset, neural_network, 
                                                context, &mpi_nn_context, &score, 
                                                train_scheduler, fp_train, epoch );
        // else
            // training_score = stochastic_gradient_descent( train_dataset, neural_network, context, &score, train_scheduler, fp_train, epoch, 1 );

        // Test

        if( rank == ROOT )
            stochastic_gradient_descent( test_dataset,  neural_network, context, 
                                        &score, test_scheduler,  fp_test,  epoch, 0 );
    
        // 
    }

    free(test_scheduler);
    free(train_scheduler);
    free_mpi_neural_network_context( &mpi_nn_context );

    if( rank == ROOT ){
        printf("P%d closing\n", rank);
        fclose(fp_train);
        fclose(fp_test);
    }

    return 1;
}

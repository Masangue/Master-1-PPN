#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


#include "type.h"
#include "global.h"
#include "neural_network.h"
#include "convolution_layer.h"
#include "dataset_manager.h"
#include "store.h"  
#include "context.h"  
#include "evaluation.h" 
#include "training.h" 
#include "preprocess_image.h" 

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
    Dataset train_dataset;
    Dataset test_dataset;
    Neural_network * neural_network;

    srand(time(NULL));

    MPI_Init(&argc, &argv);

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    // load context
    create_context( &context );
    if( rank == ROOT ){
        load_context( &context, "../data/config.cfg" );
        info_context( &context );
    }

    mpi_share_context( &context );

   
    if( rank != ROOT ){
        info_context( &context );
    }


    // init and fill dataset
    train_dataset = create_dataset( &context );
    test_dataset  = create_dataset( &context );
    if( rank == ROOT ){
        init_dataset( context.train_dirs, &train_dataset, &context);
        init_dataset( context.test_dirs, &test_dataset, &context);
    }
    mpi_share_dataset( &train_dataset );
    // 
    // //  Initialise The NN
    // neural_network = create_neural_network(&context );
    //
    // // train
    // train( &context, &train_dataset, &test_dataset, neural_network );
    // // store_neural_network( &context, neural_network);
    //             
    MPI_Finalize();
    
    // free
    // free_neural_network( neural_network  );
    // free_dataset( &train_dataset ); 
    // free_dataset( &test_dataset ); 


    free_context( &context ); 

}

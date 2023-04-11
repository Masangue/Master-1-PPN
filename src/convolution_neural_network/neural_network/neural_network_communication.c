#include "neural_network.h"

////////////////////////////////////////////////////////
////               MPI FUNCTIONS                    ////
////////////////////////////////////////////////////////

int mpi_share_neural_network( Neural_network * neural_network ){

    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    // if ( P == 1 ) {
    //     return 0;
    // }


    for(u64 i = 0; i < neural_network->size ; i++){
        mpi_share_layer( &neural_network->layers[i] );
    }

    return 0;
}


int mpi_reduce_gradient( Neural_network * neural_network, 
                            Mpi_neural_network_context * mpi_nn_context ){

    int P;
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if ( P == 1 ) {
        return 0;
    }

    // backprop is from input to output layer so we need to send from the input to the ouput
    for(u64 i = 0; i < neural_network->size ; i++){
        mpi_reduce_gradient_layer( &neural_network->layers[i], mpi_nn_context );
    }

    return 0;
}


Mpi_neural_network_context create_mpi_neural_network_context(Neural_network * neural_network, int batch_size ){
    Mpi_neural_network_context mpi_nn_context;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_nn_context.rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_nn_context.P);
    int P = mpi_nn_context.P;
    int rank = mpi_nn_context.rank;

    mpi_nn_context.workload = calloc(P, sizeof(int));
    for (int i = 0; i < batch_size; i++ ) {
        mpi_nn_context.workload[ i%P ]++;
    }
    
    mpi_nn_context.displs = calloc(P, sizeof(int));
    for (int i = 1; i < P; i++ ) {
        mpi_nn_context.displs[ i ] = mpi_nn_context.displs[ i-1 ] + mpi_nn_context.workload[ i - 1 ] ;
    }

    mpi_nn_context.mpi_layer_context = 
        malloc( neural_network->size * sizeof(Mpi_layer_context));
   

    // debug log
    if(rank == MASTER_RANK)
        for (int i = 0; i < P; i++ ) 
            printf(" T%2d working on  %3d images in the batch  %d\n", i, mpi_nn_context.workload[i], batch_size );    


    return mpi_nn_context;
}

int free_mpi_neural_network_context(Mpi_neural_network_context * mpi_nn_context){
    free( mpi_nn_context->workload);
    free( mpi_nn_context->mpi_layer_context);
    free( mpi_nn_context->displs);
    return 0;
}




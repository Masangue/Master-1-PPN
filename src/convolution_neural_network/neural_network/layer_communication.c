#include "layer.h"
#include <mpi.h>
#include <string.h>

 
////////////////////////////////////////////////////////
////                  MPI                           ////
////////////////////////////////////////////////////////

int mpi_share_layer( Layer * layer ){

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    u64 size = layer->size;
    u64 next_size = layer->next_size;


    if ( P != 1 ) {

    MPI_Bcast(layer->weights, size * next_size, MPI_U64, MASTER_RANK, MPI_COMM_WORLD);
    MPI_Bcast(layer->bias, next_size , MPI_U64, MASTER_RANK, MPI_COMM_WORLD);


    }

    return 0; 
}


int mpi_reduce_gradient_layer( Layer * layer, Mpi_neural_network_context * mpi_nn_context ){

    int rank = mpi_nn_context->rank;
    // int P    = mpi_nn_context->P;
    // int workload    = mpi_nn_context->workload[rank];
    int size    = layer->size;
    int next_size    = layer->next_size;


    // MPI_Allreduce(MPI_IN_PLACE, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)  
    MPI_Allreduce(MPI_IN_PLACE,layer->bias_gradient_accumulator,
               next_size , MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    //
    MPI_Allreduce(MPI_IN_PLACE, layer->weights_gradient_accumulator, 
           next_size * size , MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);


    //
    // if( rank == MASTER_RANK ){
    //
    //     MPI_Reduce(MPI_IN_PLACE,layer->bias_gradient_accumulator,
    //                next_size , MPI_DOUBLE, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);
    //
    //     //
    //     MPI_Reduce(MPI_IN_PLACE, layer->weights_gradient_accumulator, 
    //            next_size * size , MPI_DOUBLE, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);
    // }
    // else{
    //
    //     MPI_Reduce(layer->bias_gradient_accumulator, layer->buffer,
    //                next_size , MPI_DOUBLE, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);
    //
    //     //
    //     MPI_Reduce(layer->weights_gradient_accumulator, layer->buffer, 
    //            next_size * size , MPI_DOUBLE, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);
    // }


    return 0;

}



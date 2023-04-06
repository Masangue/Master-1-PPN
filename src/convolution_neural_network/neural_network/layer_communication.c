#include "layer.h"

 
////////////////////////////////////////////////////////
////                  MPI                           ////
////////////////////////////////////////////////////////

int mpi_share_layer( Layer * layer ){

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if ( P == 1 ) {
        return 0;
    }

    u64 size = layer->size;
    u64 next_size = layer->next_size;

    if ( rank == MASTER_RANK ){

        for (int i = 0; i < P; i++) {
        if ( i == MASTER_RANK ) continue;
        MPI_Ssend(layer->weights, size * next_size, MPI_U64, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(layer->bias, next_size, MPI_U64, i, 1000, MPI_COMM_WORLD );
        }

        }
    else {
        MPI_Status sta;
        MPI_Recv(layer->weights, size * next_size, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        MPI_Recv(layer->bias, next_size, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    }
    return 0; 
}


int mpi_gather_delta_layer( Layer * layer, Mpi_neural_network_context * mpi_nn_context ){
    // envoyer les 
    int rank = mpi_nn_context->rank;
    int P    = mpi_nn_context->P;
    int workload    = mpi_nn_context->workload[rank];
    int size    = layer->size;

    // send buf * workload
    // recv buf + stride 

    if ( rank != MASTER_RANK ){
        MPI_Ssend(layer->neurons      , size * workload, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD );
        MPI_Ssend(layer->delta_neurons, size * workload, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD );
    }

    else {
        MPI_Status sta;
        for (int i = 0; i < P; i++) {
            if ( i == MASTER_RANK ) continue;
            int displs   = mpi_nn_context->displs[i];
            int workload = mpi_nn_context->workload[i];
            MPI_Recv(layer->neurons + displs*size      , size * workload, MPI_U64, i, 1000, MPI_COMM_WORLD, &sta  );
            MPI_Recv(layer->delta_neurons + displs*size, size * workload, MPI_U64, i, 1000, MPI_COMM_WORLD, &sta  );
        }
    }
    return 0;
}



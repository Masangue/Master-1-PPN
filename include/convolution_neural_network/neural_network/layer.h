#pragma once 
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "context.h"


#include "activation.h"


typedef void activation_function_t(f64 * src, f64 * dst, u64 size) ;
typedef struct {
    u64   size;
    u64   next_size;
    f64 * weights;
    f64 * bias;
    f64 * delta_weights;
    f64 * delta_bias;

    f64 * neurons;
    f64 * delta_neurons;

} Layer;

typedef struct {
    int a;
} Mpi_layer_context;

typedef struct {
    int * workload;
    int * displs;
    int rank;
    int P;
    Mpi_layer_context * mpi_layer_context;

} Mpi_neural_network_context;





//layer
void    create_layer( Layer * layer, u64 size, u64 next_size, u64 batch_size );
void    init_layer( Layer * layer, u64 batch_size );

//forward
void    compute_layer( Layer * layer1, Layer * layer2, u64 batch_iteration, activation_function_t * activation );

//backward
f64     compute_output_delta( Layer * layer, f64 * expected, u64 batch_iteration );
void    compute_delta( Layer * layer1, Layer * layer2, u64 batch_iteration, activation_function_t * activation );
void    backpropagate( Layer * layer1, Layer * layer2, f64 eta_, f64 alpha_, u64 batch_size );

// gradient
f64 get_weight_gradient( Layer * layer1, Layer * layer2, f64 batch_size, u64 i, u64 j );
f64 get_bias_gradient( Layer * layer2, f64 batch_size, u64 j );

// mpi
int mpi_share_layer( Layer * layer );
// int mpi_gather_delta_layer( Layer * layer, Mpi_layer_context * mpi_layer_context );
int mpi_gather_delta_layer( Layer * layer, Mpi_neural_network_context * mpi_nn_context );

// debug
void    debug( Layer * layer );


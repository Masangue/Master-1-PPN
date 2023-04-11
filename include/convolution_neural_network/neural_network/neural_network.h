#pragma once 
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "type.h"
#include "context.h"

#include "activation.h"
#include "preprocess_image.h"

#include "layer.h"


typedef struct {
    Layer * layers;
    u64 size;
    f64 * expected;
    // liste activation
    activation_function_t ** activation_function;
    activation_function_t ** activation_d_function;

} Neural_network;



//neural network
Neural_network* create_neural_network( Context * context);
void    free_neural_network( Neural_network * neural_network );
void    set_input_output(Neural_network * neural_network, u8 * input, int * output );

// stochastic functions
void    stochastic_forward_compute( Neural_network * neural_network, Context * context );
void    stochastic_backward_compute( Neural_network * neural_network, Context * context );

// batch functions
void    batch_forward_propagation( Neural_network * neural_network , Context * context, u64 batch_iteration);
void    batch_backward_propagation( Neural_network * neural_network, Context * context, u64 batch_size);

void    update_batch_pointer(Neural_network * neural_network, u64 offset );
// void gather_gradient( Neural_network * neural_network, u64 batch_size );


// activation
void    prepare_activation( Neural_network * neural_network, Context * context);

// mpi
Mpi_neural_network_context create_mpi_neural_network_context(Neural_network * neural_network, int batch_size );
int free_mpi_neural_network_context(Mpi_neural_network_context * mpi_nn_context);

int mpi_share_neural_network( Neural_network * neural_network );
int mpi_gather_delta_neural_network( Neural_network * neural_network, 
                                    Mpi_neural_network_context * mpi_nn_context );
int mpi_reduce_gradient( Neural_network * neural_network, 
                            Mpi_neural_network_context * mpi_nn_context );

//misc
void    shuffle(u64 size, u64 * tab);
void    range(u64 size, u64 * tab);



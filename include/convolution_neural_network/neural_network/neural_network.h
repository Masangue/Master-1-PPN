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
    f64 * neurons;
    f64 * weights;
    f64 * bias;
    f64 * delta_neurons;
    f64 * delta_weights;
    f64 * delta_bias;
} Layer;


typedef struct {
    Layer * layers;
    u64 size;
    f64 * expected;
    // liste activation
    activation_function_t ** activation_function;
    activation_function_t ** activation_d_function;

} Neural_network;



//neural network
Neural_network* init_neural_network( Context * context);
void    forward_compute( Neural_network * neural_network, Context * context );
void    backward_compute( Neural_network * neural_network, Context * context );
void    free_neural_network( Neural_network * neural_network, u64 size);



//layer
void    init_layer( Layer * layer, u64 next_size );
void create_layer( Layer * layer, u64 size, u64 next_size );

//forward
void    fill_input(Layer * layer, u64 size, u8 * tab);
void    compute_layer( Layer * layer1, Layer * layer2, activation_function_t * activation );
f64     get_error(Layer * layer, f64 * expected);

//backward
f64     compute_output_delta( Layer * layer, f64 * expected, activation_function_t * activation );
void    compute_delta( Layer * layer1, Layer * layer2, activation_function_t * activation );
void    backpropagate( Layer * layer1, Layer * layer2, f64 eta_, f64 alpha_ );



// debug
void    debug( Layer * layer, u64 next_size );



//misc
void    shuffle(u64 size, u64 * tab);
void    range(u64 size, u64 * tab);

void prepare_activation( Neural_network * neural_network, Context * context);




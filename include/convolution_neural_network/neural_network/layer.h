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
    f64 * weights;
    f64 * bias;
    f64 * delta_weights;
    f64 * delta_bias;

    f64 * neurons;
    f64 * delta_neurons;

    f64 * batch_delta_neurons;
    f64 * batch_neurons; 
} Layer;



//layer
void    create_layer( Layer * layer, u64 size, u64 next_size, u64 batch_size );
void    init_layer( Layer * layer, u64 next_size, u64 batch_size );

//forward
void    compute_layer( Layer * layer1, Layer * layer2, activation_function_t * activation );

//backward
f64     compute_output_delta( Layer * layer, f64 * expected );
void    compute_delta( Layer * layer1, Layer * layer2, activation_function_t * activation );
void    backpropagate( Layer * layer1, Layer * layer2, f64 eta_, f64 alpha_, u64 batch_size );

// gradient
f64 get_weight_gradient( Layer * layer1, Layer * layer2, f64 batch_size, u64 i, u64 j );
f64 get_bias_gradient( Layer * layer2, f64 batch_size, u64 j );

// debug
void    debug( Layer * layer, u64 next_size );




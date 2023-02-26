#pragma once 
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "context.h"

// #define eta 0.5
// #define alpha2 0.3

typedef struct {
    u64   size;
    f64 * neurons;
    f64 * weights;
    f64 * bias;
    f64 * delta_neurons;
    f64 * delta_weights;
    f64 * delta_bias;
} Layer;

// f64 alpha = 0.9;
// f64 eta = 0.3;

//neural network
Layer** init_neural_network(int * neurons_per_layers, u64 nb_layers);
void    forward_compute(u64 nb_layers, Layer ** layers, Context * context );
void    backward_compute(Layer ** layers, f64 * expected, Context * context );
void    free_neural_network(Layer ** layers, u64 size);



//layer
void    init_layer( Layer * layer, u64 next_size );
Layer * create_layer( u64 size, u64 next_size );

//forwqrd
void    fill_input(Layer * layer, u64 size, u8 * tab);
void    compute_layer( Layer * layer1, Layer * layer2 );
f64     get_error(Layer * layer, f64 * expected);

//backward
f64     compute_Output_delta( Layer * layer, f64 * expected ); 
void    compute_delta( Layer * layer1, Layer * layer2 );
void    backpropagate( Layer * layer1, Layer * layer2, f64 eta_, f64 alpha_ );



// debug
void    debug( Layer * layer, u64 next_size );




//misc
void    shuffle(u64 size, u64 * tab);

// activations functions
f64     sigmoid(f64 x);
f64     d_sigmoid(f64 x);




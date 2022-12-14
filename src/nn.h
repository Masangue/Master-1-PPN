#pragma once 
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"

#define eta 0.5
#define alpha2 0.3

typedef struct {
    u64 size;
    f64 * neurons;
    f64 * weights;
    f64 * bias;
    f64 * delta_neurons;
    f64 * delta_weights;
    f64 * delta_bias;
} Layer;

// f64 alpha = 0.9;
// f64 eta = 0.3;

Layer * createLayer( u64 size, u64 next_size );
void    free_all(Layer ** layers, u64 size);
void    initLayer( Layer * layer, u64 next_size );
void    computeLayer( Layer * layer1, Layer * layer2 );
f64     computeOutputDelta( Layer * layer, f64 * expected );
void    computeDelta( Layer * layer1, Layer * layer2 );
void    backpropagate( Layer * layer1, Layer * layer2 );
void    debug( Layer * layer, u64 next_size );
void    fill_input(Layer * layer, u64 size, u8 * tab);
void    forward_compute(u64 nb_layers, Layer ** layers );
void    backward_compute(u64 nb_layers, Layer ** layers, f64 * expected );
f64     get_error(Layer * layer, f64 * expected);

Layer ** Init_Neural_network(u64 * neurons_per_layers, u64 nb_layers);


//misc
void shuffle(u64 size, u64 * tab);

// activations functions
f64 sigmoid(f64 x);
f64 d_sigmoid(f64 x);




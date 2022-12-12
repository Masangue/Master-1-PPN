#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"

#define alpha 0.9
#define eta 0.2

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
void    initLayer( Layer * layer, u64 next_size );
void    computeLayer( Layer * layer1, Layer * layer2 );
f64     computeOutputDelta( Layer * layer, f64 * expected );
void    computeDelta( Layer * layer1, Layer * layer2 );
void    Backpropagate( Layer * layer1, Layer * layer2 );


// activations functions
f64 sigmoid(f64 x);
f64 d_sigmoid(f64 x);




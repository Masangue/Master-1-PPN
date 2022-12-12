#include "type.h"

typedef struct {
    u64 size;
    f64 * neuron;
    f64 * weights;
    f64 * bias;
    f64 * delta_neurons;
    f64 * delta_weights;
    f64 * delta_bias;
} Layer;


Layer * createLayer( u64 size );
void    initLayer( Layer * layer, u64 size_NextLayer );
void    computeLayer( Layer * layer1, Layer * layer2 );
void    computeDelta( Layer * layer1, Layer * layer2 );
void    Backpropagate( Layer * layer1, Layer * layer2 );




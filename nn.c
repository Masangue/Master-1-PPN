#include "nn.h"

// activations functions
f64 sigmoid(f64 x) {
    return 1 / (1 + exp(-x) );
}

f64 d_sigmoid(f64 x) {
    return x * (1 - x);
}


Layer * createLayer( u64 size, u64 next_size ){
    Layer * layer;
    layer = aligned_alloc(64, size * sizeof(Layer) );
    layer->neurons       = aligned_alloc(64, size *             sizeof(Layer) );
    layer->weights       = aligned_alloc(64, size * next_size * sizeof(f64) );
    layer->bias          = aligned_alloc(64, next_size *        sizeof(f64) );

    layer->delta_neurons = aligned_alloc(64, size *             sizeof(f64) );
    layer->delta_weights = aligned_alloc(64, size * next_size * sizeof(f64) );
    layer->delta_bias    = aligned_alloc(64, next_size *        sizeof(f64) );

    return layer;
}

void initLayer( Layer * layer, u64 next_size ){
    u64 size = layer->size;
    for( u64 j = 0; j < next_size; j++ ){
        layer->bias[j]       = ((f64) rand() / (f64)RAND_MAX) - 0.5;
        layer->delta_bias[j] = 0.0f;
        for( u64 i = 0; i < size ; i++ ){
            layer->weights[j * size + i] = ((f64) rand() / (f64)RAND_MAX) - 0.5;
            layer->delta_weights[j * size + i] = 0.0f;
        }   
    }
}


void computeLayer( Layer * layer1, Layer * layer2 ){
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    f64 s = 0.f;

    for( u64 j = 0; j < next_size; j++ ){
            s = layer1->bias[j];
            for( u64 i = 0; i < size ; i++ ){
                s += layer1->neurons[i] * layer1->weights[ j * size + i ];
            }
            layer1->neurons[j] = sigmoid(s);
        }
     
}

f64 computeOutputDelta( Layer * layer, f64 * expected ){
    u64 size = layer->size;
    f64 err = 0.f;

    for( u64 i = 0; i < size; i++ ){
            err += 0.5 * pow( expected[i] - layer->neurons[i], 2 );

            layer->delta_neurons[i] = ( expected[i] - layer->neurons[i] ) 
                                * d_sigmoid( layer->neurons[i] );
        }

    return err;
}

void computeDelta( Layer * layer1, Layer * layer2 ){

    u64 size = layer1->size;
    u64 next_size = layer2->size;
    f64 s = 0.f;

    for( u64 i = 0; i < size; i++ ){
        s = 0.0;
        for( u64 j = 0; j < next_size; j++ ){
            s += layer2->weights[j * size + i] * layer2->delta_neurons[j];
        }
        layer1->delta_neurons[i] = s * d_sigmoid( layer1->neurons[i] ) ;
    }

}


void Backpropagate( Layer * layer1, Layer * layer2 ){
    
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    
    for( u64 j = 0; j < size; j++){
        layer2->delta_bias[j] = eta * layer2->delta_neurons[j] + alpha * layer2->delta_bias[j];
        layer2->bias[j] += layer2->delta_bias[j];
        for( u64 i = 0; i < next_size; i++){
            layer1->delta_weights[j * size + i] = eta * layer1->neurons[i] * layer2->delta_neurons[j] + 
                                                  alpha * layer1->weights[j * size + i];
            layer1->weights[ j * size + i] += layer1->delta_weights[j * size + i];
        }
    }
}




#include "nn.h"

// activations functions
f64 sigmoid(f64 x) {
    return 1 / (1 + exp(-x) );
}

f64 d_sigmoid(f64 x) {
    return x * (1 - x);
}

// allocate a layer
Layer * createLayer( u64 size, u64 next_size ){
    Layer * layer;
    layer = aligned_alloc(64, size * sizeof(Layer) );
    layer->size = size;

    layer->neurons       = aligned_alloc(64, size *             sizeof(f64) );
    layer->weights       = aligned_alloc(64, size * next_size * sizeof(f64) );
    layer->bias          = aligned_alloc(64, next_size *        sizeof(f64) );

    layer->delta_neurons = aligned_alloc(64, size *             sizeof(f64) );
    layer->delta_weights = aligned_alloc(64, size * next_size * sizeof(f64) );
    layer->delta_bias    = aligned_alloc(64, next_size *        sizeof(f64) );

    return layer;
}


// init a layer with random value
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
    for( u64 i = 0; i < size ; i++ ){
        layer->neurons[i]       = 0;
        layer->delta_neurons[i] = 0;

    }
}


// 
void computeLayer( Layer * layer1, Layer * layer2 ){
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    f64 s = 0.0f;

    for( u64 j = 0; j < next_size; j++ ){
            s = layer1->bias[j];
            for( u64 i = 0; i < size ; i++ ){
                s += layer1->neurons[i] * layer1->weights[ j * size + i ];
            }
            layer2->neurons[j] = sigmoid(s);
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

    return sqrt(pow(expected[0] - layer->neurons[0],2));
}

void computeDelta( Layer * layer1, Layer * layer2 ){

    u64 size = layer1->size;
    u64 next_size = layer2->size;
    f64 s = 0.f;

    for( u64 i = 0; i < size; i++ ){
        s = 0.0;
        for( u64 j = 0; j < next_size; j++ ){
            s += layer1->weights[j * size + i] * layer2->delta_neurons[j];
        }
        layer1->delta_neurons[i] = s * d_sigmoid( layer1->neurons[i] ) ;
    }

}


void backpropagate( Layer * layer1, Layer * layer2 ){
    
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    
    for( u64 j = 0; j < next_size; j++){
        layer1->delta_bias[j] = eta * layer2->delta_neurons[j] + alpha2 * layer1->delta_bias[j];
        layer1->bias[j] += layer1->delta_bias[j];
        for( u64 i = 0; i < size; i++){
            layer1->delta_weights[j * size + i] = eta * layer1->neurons[i] * layer2->delta_neurons[j] + 
                                                  alpha2 * layer1->delta_weights[j * size + i];
            layer1->weights[ j * size + i] += layer1->delta_weights[j * size + i];
        }
    }
}

void debug( Layer * layer, u64 next_size ){
    printf("\n\n");
    printf("neurons : \n");
    for( u64 i = 0; i < layer->size; i++)
        printf("%lf \n", layer->neurons[i]);
    
    // printf("\n\n");
    // printf("weights : \n");
    // for( u64 j = 0; j < next_size; j++){
    //     for( u64 i = 0; i < layer->size; i++){
    //         printf("%lf \n", layer->weights[ j * layer->size + i]);
    //     }
    // }
    
    printf("\n\n");
    printf("delta weights : \n");
    for( u64 j = 0; j < next_size; j++){
        for( u64 i = 0; i < layer->size; i++){
            printf("%lf \n", layer->delta_weights[ j * layer->size + i]);
        }
    }

    // printf("\n\n");
    // printf("delta neurons : \n");
    // for( u64 i = 0; i < layer->size; i++)
    //     printf("%lf \n", layer->delta_neurons[i]);
    

}

void free_all(Layer ** layers, u64 size)
{
    for(int i = 0; i<size; i++){
        free(layers[i]->bias);
        free(layers[i]->neurons);
        free(layers[i]->weights);

        free(layers[i]->delta_neurons);
        free(layers[i]->delta_weights);
        free(layers[i]->delta_bias);
        free(layers[i]);
    }
    free(layers);
}

Layer ** Init_Neural_network(u64 * neurons_per_layers, u64 nb_layers){
    Layer ** layers = malloc( nb_layers * sizeof(Layer *) );

    for(u64 i = 0; i < nb_layers; i++){
        layers[i] = createLayer( neurons_per_layers[i], neurons_per_layers[i+1]);
    }

    for(u64 i = 0; i < nb_layers - 1; i++){
        initLayer( layers[i], neurons_per_layers[i+1] );
    }

    return layers;
}




void shuffle(u64 size, u64 * tab){
    for( u64 p = 0 ; p < size ; p++ ) { 
        tab[p] = p ;
    }
    for( u64 p = 0 ; p < size - 1 ; p++) {
        u64 np = rand() % ( size - p) + p;
            
        u64 op = tab[p] ;
        tab[p] = tab[np];
        tab[np] = op;
    }
}


void fill_input(Layer * layer, u64 size, u8 * tab){
    for( u64 i = 0; i < size; i++ ){
        layer->neurons[i] = (f64) tab[i] / 20;
    }
}

void forward_compute(u64 nb_layers, Layer ** layers ){
    for(u64 i = 0; i < nb_layers - 1; i++){
        computeLayer( layers[i], layers[i+1] );
    }
}

f64 get_error(Layer * layer, f64 * expected){
    u64 size = layer->size;
    f64 err = 0.f;

    for( u64 i = 0; i < size; i++ ){
            err += sqrt(pow(expected[i] - layer->neurons[i],2));
        }

    return err/size;

}

void backward_compute(u64 nb_layers, Layer ** layers, f64 * expected ){

    computeOutputDelta( layers[nb_layers - 1] , expected );

    for(u64 i = nb_layers - 2; i > 0; i--){
        computeDelta( layers[i], layers[i+1] );
    }

    for(u64 i = 0; i < nb_layers - 1; i++){
        backpropagate( layers[i], layers[i+1] );
    }

}

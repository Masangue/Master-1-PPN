#include "neural_network.h"

//  Activations functions
f64 sigmoid(f64 x) {
    return 1 / (1 + exp(-x) );
}

f64 d_sigmoid(f64 x) {
    return x * (1 - x);
}

//  Allocate a layer
Layer * create_layer( u64 size, u64 next_size ){
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


//  Init a layer with random values
void init_layer( Layer * layer, u64 next_size ){
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


//  Applies standard neural computation function
void compute_layer( Layer * layer1, Layer * layer2 ){
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

//  First step of the backpropagation process
//  Computes the output error. 
f64 compute_output_delta( Layer * layer, f64 * expected ){
    u64 size = layer->size;
    f64 err = 0.f;

    for( u64 i = 0; i < size; i++ ){
            err += 0.5 * pow( expected[i] - layer->neurons[i], 2 );

            layer->delta_neurons[i] = ( expected[i] - layer->neurons[i] ) 
                                * d_sigmoid( layer->neurons[i] );
        }

    return sqrt(pow(expected[0] - layer->neurons[0],2));
}

//  Backpropagation process.
//  Computes the error delta for all neurons of a layer
//  This function will be called for each layer
void compute_delta( Layer * layer1, Layer * layer2 ){

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

//  Backpropagation process
//  Changes the weights of all neurons of a layer
void backpropagate( Layer * layer1, Layer * layer2, f64 eta_, f64 alpha_ ){
    
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    
    for( u64 j = 0; j < next_size; j++){
        layer1->delta_bias[j] = eta_ * layer2->delta_neurons[j] + alpha_ * layer1->delta_bias[j];
        layer1->bias[j] += layer1->delta_bias[j];
        for( u64 i = 0; i < size; i++){
            layer1->delta_weights[j * size + i] = eta_ * layer1->neurons[i] * layer2->delta_neurons[j] + 
                                                  alpha_ * layer1->delta_weights[j * size + i];
            layer1->weights[ j * size + i] += layer1->delta_weights[j * size + i];
        }
    }
}

// Debugging function
// Prints the whole NN in a visually clear format
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

//  General free function, liberating all allocated memory to the NN
void free_neural_network(Layer ** layers, u64 size)
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

//  Creates and initializes the NN by calling previously defined functions
Layer ** init_neural_network(int * neurons_per_layers, u64 nb_layers){
    Layer ** layers = malloc( nb_layers * sizeof(Layer *) );

    for(u64 i = 0; i < nb_layers; i++){
        layers[i] = create_layer( neurons_per_layers[i], neurons_per_layers[i+1]);
    }

    for(u64 i = 0; i < nb_layers - 1; i++){
        init_layer( layers[i], neurons_per_layers[i+1] );
    }

    return layers;
}

//  Shuffles the dataset to prevents pattern redundancy
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

//  Fills the input layers with the list of chars representing the image
void fill_input(Layer * layer, u64 size, u8 * tab){
    for( u64 i = 0; i < size; i++ ){
        layer->neurons[i] = (f64) tab[i] / 255;
    }
}

//  Wrapper function, computing each layer forward
void forward_compute(u64 nb_layers, Layer ** layers, Context * context ){
    for(u64 i = 0; i < nb_layers - 1; i++){
        compute_layer( layers[i], layers[i+1] );
    }
}

//  Computes the output error, used to compute the cumulated error of the NN
f64 get_error(Layer * layer, f64 * expected){
    u64 size = layer->size;
    f64 err = 0.f;

    for( u64 i = 0; i < size; i++ ){
            err += sqrt(pow(expected[i] - layer->neurons[i],2));
        }

    return err/size;

}

//  Main function of the backpropagation process
//  Calls the three other backpropagation functions
void backward_compute(Layer ** layers, f64 * expected, Context * context ){
    
    u64 nb_layers = context->nn_size;
    compute_output_delta( layers[nb_layers - 1] , expected );

    for(u64 i = nb_layers - 2; i > 0; i--){
        compute_delta( layers[i], layers[i+1] );
    }

    for(u64 i = 0; i < nb_layers - 1; i++){
        backpropagate( layers[i], layers[i+1], context->eta_, context->alpha_ );
    }

}

#include "neural_network.h"



////////////////////////////////////////////////////////
////          INTIT / CREATE FUNCTIONS              ////
////////////////////////////////////////////////////////


//  Allocate a layer
void create_layer( Layer * layer, u64 size, u64 next_size ){
    layer->size = size;

    layer->neurons       = aligned_alloc(64, size *             sizeof(f64) );
    layer->weights       = aligned_alloc(64, size * next_size * sizeof(f64) );
    layer->bias          = aligned_alloc(64, next_size *        sizeof(f64) );

    layer->delta_neurons = aligned_alloc(64, size *             sizeof(f64) );
    layer->delta_weights = aligned_alloc(64, size * next_size * sizeof(f64) );
    layer->delta_bias    = aligned_alloc(64, next_size *        sizeof(f64) );

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




//  General free function, liberating all allocated memory to the NN
void free_neural_network(Neural_network * neural_network, u64 size)
{
    for(int i = 0; i<size; i++){
        Layer layer = neural_network->layers[i];
        free(layer.bias);
        free(layer.neurons);
        free(layer.weights);

        free(layer.delta_neurons);
        free(layer.delta_weights);
        free(layer.delta_bias);
        // free(layer);
    }
    free( neural_network->layers );
    free( neural_network->expected );
    free( neural_network );
}

//  Creates and initializes the NN by calling previously defined functions
Neural_network * init_neural_network(int * neurons_per_layers, u64 nn_size ){

    Neural_network * neural_network = malloc( sizeof(Neural_network) );
    neural_network->size = nn_size;
    neural_network->layers = malloc( nn_size * sizeof(Layer) );
    neural_network->expected = malloc( neurons_per_layers[nn_size - 1] * sizeof( f64 ) );
    
    neural_network->activation_function = malloc( nn_size * sizeof(activation_function_t*) );
    neural_network->activation_d_function = malloc( nn_size * sizeof(activation_function_t*) );

    for(u64 i = 0; i < nn_size; i++){
        create_layer( &neural_network->layers[i], 
                    neurons_per_layers[i], neurons_per_layers[i+1] );
    }

    for(u64 i = 0; i < nn_size - 1; i++){
        init_layer( &neural_network->layers[i], neurons_per_layers[i+1] );
    }

    return neural_network;
}













////////////////////////////////////////////////////////
////              FORWARD FUNCTIONS                 ////
////////////////////////////////////////////////////////


//  Fills the input layers with the list of chars representing the image
void fill_input(Layer * layer, u64 size, u8 * tab){
    for( u64 i = 0; i < size; i++ ){
        layer->neurons[i] = (f64) tab[i] / 255;
    }
}


//  Applies standard neural computation function
void compute_layer( Layer * layer1, Layer * layer2, activation_function_t * activation ){
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    f64 s = 0.0f;

    for( u64 j = 0; j < next_size; j++ ){
        s = layer1->bias[j];
        for( u64 i = 0; i < size ; i++ ){
            s += layer1->neurons[i] * layer1->weights[ j * size + i ];
        }
        // layer2->neurons[j] = sigmoid(s);
        layer2->neurons[j] = s;
    }

    activation( layer2->neurons, layer2->neurons, next_size );
     
}





////////////////////////////////////////////////////////
////              BACKWARD FUNCTIONS                ////
////////////////////////////////////////////////////////



//  First step of the backpropagation process
//  Computes the output error. 
f64 compute_output_delta( Layer * layer, f64 * expected, activation_function_t * activation ){
    u64 size = layer->size;
    for( u64 i = 0; i < size; i++ ){

        // layer->delta_neurons[i] = ( expected[i] - layer->neurons[i] ) 
                                // * d_sigmoid( layer->neurons[i] );
        layer->delta_neurons[i] = ( expected[i] - layer->neurons[i] ) ;
    }

    //delta_neurons = delta_neurons * d_sigmoid( neurons );
    activation( layer->neurons, layer->delta_neurons, size );
    return 0;
}

//  Backpropagation process.
//  Computes the error delta for all neurons of a layer
//  This function will be called for each layer
void compute_delta( Layer * layer1, Layer * layer2, activation_function_t * activation ){

    u64 size = layer1->size;
    u64 next_size = layer2->size;
    f64 s = 0.f;

    for( u64 i = 0; i < size; i++ ){
        s = 0.0;
        for( u64 j = 0; j < next_size; j++ ){
            s += layer1->weights[j * size + i] * layer2->delta_neurons[j];
        }
        // layer1->delta_neurons[i] = s * d_sigmoid( layer1->neurons[i] ) ;
        layer1->delta_neurons[i] = s ;
    }

    activation( layer1->neurons,  layer1->delta_neurons, size );

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


////////////////////////////////////////////////////////
////                   WRAPPER                      ////
////////////////////////////////////////////////////////



//  Wrapper function, computing each layer forward
void forward_compute( Neural_network * neural_network , Context * context ){
    for(u64 i = 0; i < neural_network->size - 1; i++){
        compute_layer( &neural_network->layers[i], &neural_network->layers[i+1],
                      neural_network->activation_function[i] );
    }
}

//  Main function of the backpropagation process
//  Calls the three other backpropagation functions
void backward_compute( Neural_network * neural_network, Context * context ){
    
    u64 nn_size = neural_network->size;
    compute_output_delta( &neural_network->layers[nn_size - 1] , 
                         neural_network->expected, 
                         neural_network->activation_d_function[nn_size - 1] );

    for(u64 i = nn_size - 2; i > 0; i--){
        compute_delta( &neural_network->layers[i], &neural_network->layers[i+1],
                         neural_network->activation_d_function[i] );
    }

    for(u64 i = 0; i < nn_size - 1; i++){
        backpropagate( &neural_network->layers[i], &neural_network->layers[i+1],
                      context->eta_, context->alpha_ );
    }

}

////////////////////////////////////////////////////////
////               DEBUG / TOOLS                    ////
////////////////////////////////////////////////////////



// Debugging function
// Prints the whole NN in a visually clear format
void debug( Layer * layer, u64 next_size ){
    printf("\n\n");
    // printf("neurons : \n");
    // for( u64 i = 0; i < layer->size; i++)
    //     printf("%lf ", layer->neurons[i]);
    // 
    // printf("\n\n");
    // printf("weights : \n");
    // for( u64 j = 0; j < next_size; j++){
    //     for( u64 i = 0; i < layer->size; i++){
    //         printf("%lf ", layer->weights[ j * layer->size + i]);
    //     }
    //     // break;
    // }
    
    // printf("\n\n");
    // printf("delta weights : \n");
    // for( u64 j = 0; j < next_size; j++){
    //     for( u64 i = 0; i < layer->size; i++){
    //         printf("%lf ", layer->delta_weights[ j * layer->size + i]);
    //     }
    // }

    printf("\n\n");
    printf("delta neurons : \n");
    for( u64 i = 0; i < layer->size; i++)
        printf("%lf \n", layer->delta_neurons[i]);
    

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


void    range(u64 size, u64 * tab){
    for( u64 p = 0 ; p < size ; p++ ) { 
        tab[p] = p ;
    }
}



// delete this function ??
//  Computes the output error, used to compute the cumulated error of the NN
f64 get_error(Layer * layer, f64 * expected){
    u64 size = layer->size;
    f64 err = 0.f;

    for( u64 i = 0; i < size; i++ ){
            err += sqrt(pow(expected[i] - layer->neurons[i],2));
        }

    return err/size;

}


// activation_function_t * activation_map( char * str_func ){
//     if(strcmp(str_func, "sig")){
//         return &apply_sigmoid;
//     }
//     else if(strcmp(str_func, "relu")){
//         return &apply_sigmoid;
//     }
//     else {
//         return &apply_sigmoid;
//         }
// }

void prepare_activation( Neural_network * neural_network, Context * context){
    u64 nn_size = neural_network->size;
    for (u64 i = 0; i < nn_size; i++ ) {
        if(strcmp(context->activation_functions[i], "sig")==0){
            neural_network->activation_function[i] = &apply_sigmoid;
            neural_network->activation_d_function[i] = &apply_d_sigmoid;
        }
        else if(strcmp(context->activation_functions[i], "relu")==0){
            neural_network->activation_function[i] = &apply_relu;
            neural_network->activation_d_function[i] = &apply_d_relu;
        }
        else {
            neural_network->activation_function[i] = &apply_sigmoid;
            neural_network->activation_d_function[i] = &apply_d_sigmoid;
        }
    }
    
}

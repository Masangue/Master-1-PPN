#include "neural_network.h"
#include <stdio.h>



////////////////////////////////////////////////////////
////          INTIT / CREATE FUNCTIONS              ////
////////////////////////////////////////////////////////


//  Allocate a layer
void create_layer( Layer * layer, u64 size, u64 next_size, u64 batch_size ){
    layer->size = size;
    layer->weights       = aligned_alloc(64, size * next_size  * sizeof(f64) );
    layer->bias          = aligned_alloc(64, next_size *         sizeof(f64) );

    layer->delta_weights = aligned_alloc(64, size * next_size  * sizeof(f64) );
    layer->delta_bias    = aligned_alloc(64, next_size *         sizeof(f64) );


    layer->batch_neurons       = aligned_alloc(64, size * batch_size * sizeof(f64) );
    layer->batch_delta_neurons = aligned_alloc(64, size * batch_size * sizeof(f64) );

    layer->neurons = layer->batch_neurons;
    layer->delta_neurons = layer->batch_delta_neurons;

}


//  Init a layer with random values
void init_layer( Layer * layer, u64 next_size, u64 batch_size ){
    u64 size = layer->size;
    for( u64 j = 0; j < next_size; j++ ){
        layer->bias[j]       = ((f64) rand() / (f64)RAND_MAX) - 0.5;
        layer->delta_bias[j] = 0.0f;
        for( u64 i = 0; i < size ; i++ ){
            layer->weights[j * size + i] = ((f64) rand() / (f64)RAND_MAX) - 0.5;
            layer->delta_weights[j * size + i] = 0.0f;
        }   
    }
    for( u64 i = 0; i < size * batch_size; i++ ){
        layer->batch_neurons[i]       = 0;
        layer->batch_delta_neurons[i] = 0;

    }
}




//  General free function, liberating all allocated memory to the NN
void free_neural_network(Neural_network * neural_network )
{
    for(int i = 0; i < neural_network->size ; i++){
        Layer layer = neural_network->layers[i];
        free(layer.bias);
        free(layer.batch_neurons);
        free(layer.weights);

        free(layer.batch_delta_neurons);
        free(layer.delta_weights);
        free(layer.delta_bias);
        // free(layer);
    }
    free( neural_network->layers );
    free( neural_network->expected );
    free( neural_network );
}

//  Creates and initializes the NN by calling previously defined functions
Neural_network * init_neural_network( Context * context ){
    
    u64 nn_size = context->nn_size;
    u64 input_size = input_size_from_context(context );
    u64 output_size = 1;

    // topology
    u64 * topology = malloc( (nn_size+1)  * sizeof(u64) );
    topology[0] = input_size;
    for(u64 i = 1; i < nn_size-1; i++){
        topology[i] = context->topology[i-1];
    }
    topology[nn_size-1] = output_size;
    topology[nn_size] = 1;

    
    Neural_network * neural_network = malloc( sizeof(Neural_network) );
    neural_network->size = nn_size;
    neural_network->layers = malloc( nn_size  * sizeof(Layer) );
    neural_network->expected = malloc( output_size * sizeof( f64 ) );
    
    neural_network->activation_function = malloc( nn_size * sizeof(activation_function_t*) );
    neural_network->activation_d_function = malloc( nn_size * sizeof(activation_function_t*) );

    // layers
    for(u64 i = 0; i < nn_size; i++){
        create_layer( &neural_network->layers[i], 
                    topology[i], topology[i+1], context->batch_size );
    }

    for(u64 i = 0; i < nn_size - 1; i++){
        init_layer( &neural_network->layers[i], topology[i+1], context->batch_size );
    }

    // // creation
    // create_layer( &neural_network->layers[0], input_size, topology[0] );
    // for(u64 i = 0; i < nn_size-3; i++){ //not working if nn_size = 2
    //     create_layer( &neural_network->layers[i+1], 
    //                 topology[i], topology[i+1] );
    // }
    // create_layer( &neural_network->layers[nn_size-2], topology[nn_size-3], output_size );
    // create_layer( &neural_network->layers[nn_size-1], output_size, output_size );
    //
    // 
    // // init
    // init_layer( &neural_network->layers[0], topology[0] );
    // for(u64 i = 0; i < nn_size - 2; i++){
    //     init_layer( &neural_network->layers[i+1], topology[i+1] );
    // }
    // init_layer( &neural_network->layers[nn_size-1], output_size );
    //
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
        // layer->delta_neurons[i] =  - ( expected[i] - layer->neurons[i] ) ;
        layer->delta_neurons[i] =  layer->neurons[i] - expected[i]   ;
    }

    //delta_neurons = delta_neurons * d_sigmoid( neurons );
    // activation( layer->neurons, layer->delta_neurons, size );
    // printf("expected  : %f ---  neuron : %f ---  deltaNeuron : %f\n", expected[0] , layer->neurons[0], layer->delta_neurons[0]);
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

    //delta_neurons = delta_neurons * d_sigmoid( neurons );
    activation( layer1->neurons,  layer1->delta_neurons, size );

}


f64 get_weight_gradient( Layer * layer1, Layer * layer2, f64 batch_size, u64 i, u64 j ){

    u64 size = layer1->size;
    u64 next_size = layer2->size;
     
    f64 weight_gradient = 0;
    for(int k = 0; k < batch_size; k++){
        weight_gradient += layer1->batch_neurons[i + k * size] * layer2->batch_delta_neurons[ j + k * next_size ];
    }
    return weight_gradient = weight_gradient / batch_size;

}

f64 get_bias_gradient( Layer * layer2, f64 batch_size, u64 j ){

    u64 next_size = layer2->size;
     
    f64 bias_gradient = 0;
        for(int k = 0; k < batch_size; k++){
            bias_gradient += layer2->batch_delta_neurons[ j + k * next_size ];
        }

    return bias_gradient = bias_gradient / batch_size;

}


//  Backpropagation process
//  Changes the weights of all neurons of a layer
void backpropagate( Layer * layer1, Layer * layer2, f64 eta_, f64 alpha_, u64 batch_size ){
    
    u64 size = layer1->size;
    u64 next_size = layer2->size;
     
    for( u64 j = 0; j < next_size; j++){

        f64 bias_gradient = get_bias_gradient(layer2, batch_size, j);
        layer1->delta_bias[j] = (1.f/size) * eta_ * bias_gradient + alpha_ * layer1->delta_bias[j];
        layer1->bias[j] -= layer1->delta_bias[j];

        //
        for( u64 i = 0; i < size; i++){

            f64 weight_gradient =  get_weight_gradient( layer1, layer2, batch_size, i, j);
            layer1->delta_weights[j * size + i] = (1.f/size) * eta_ * weight_gradient + 
                                                  alpha_ * layer1->delta_weights[j * size + i];
            layer1->weights[ j * size + i] -= layer1->delta_weights[j * size + i];
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
                      neural_network->activation_function[i+1] );
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
                      context->eta_, context->alpha_, 1 );
    }

}

////////////////////////////////////////////////////////
////                  BATCH FUNCTION                ////
////////////////////////////////////////////////////////



void batch_forward_propagation( Neural_network * neural_network , Context * context ){
   
    u64 nn_size = neural_network->size;

    for(u64 i = 0; i < nn_size - 1; i++){
        compute_layer( &neural_network->layers[i], &neural_network->layers[i+1],
                      neural_network->activation_function[i+1] );
    }
    
    compute_output_delta( &neural_network->layers[nn_size - 1] , 
                         neural_network->expected, 
                         neural_network->activation_d_function[nn_size - 1] );

    for(u64 i = nn_size - 2; i > 0; i--){
        compute_delta( &neural_network->layers[i], &neural_network->layers[i+1],
                         neural_network->activation_d_function[i] );
    }
}


void batch_backward_propagation( Neural_network * neural_network, Context * context, u64 batch_size ){
    
    u64 nn_size = neural_network->size;
    // gather_gradient( neural_network, batch_size );

    //
    for(u64 i = 0; i < nn_size - 1; i++){
        backpropagate( &neural_network->layers[i], &neural_network->layers[i+1],
                      context->eta_, context->alpha_, batch_size );
    }

    //
    update_batch_pointer( neural_network, 0);
    return;
}


void gather_gradient( Neural_network * neural_network, u64 batch_size ){

    for(int i = 0; i < neural_network->size ; i++){

        //
        Layer * layer = &neural_network->layers[i];
        layer->delta_neurons = layer->batch_delta_neurons + 0;
        layer->neurons = layer->batch_neurons + 0;
        
        //
        for(int j = 0; j < layer->size; j++){
            for(int k = 1; k < batch_size; k++){
                layer->delta_neurons[j] += layer->batch_delta_neurons[ j + k * layer->size ];
                layer->neurons[j] += layer->batch_neurons[ j + k * layer->size ];

                // if( i == 6 )
                    // printf("out3 : %d - %d - neurons : %f\n", i, j, layer->batch_neurons[ j + k * layer->size ]);
            }
            // if( j < 3 )
                // printf("%d - %d - neurons : %f\n", i, j, layer->neurons[j]);
            layer->delta_neurons[j] /= (f64)batch_size;
            layer->neurons[j] /= (f64)batch_size;
        }
    }
    // getchar();

    return ; 
}



//  General free function, liberating all allocated memory to the NN
void update_batch_pointer(Neural_network * neural_network, u64 offset )
{
    for(int i = 0; i < neural_network->size ; i++){
        Layer * layer = &neural_network->layers[i];
        // if( i == 6)
        //     printf("OFFSET : %lld\n", offset * layer->size);
        layer->delta_neurons = layer->batch_delta_neurons + offset * layer->size;
        layer->neurons = layer->batch_neurons + offset * layer->size;
    }
}

////////////////////////////////////////////////////////
////               DEBUG / TOOLS                    ////
////////////////////////////////////////////////////////



// Debugging function
// Prints the whole NN in a visually clear format
void debug( Layer * layer, u64 next_size ){
    printf("\n\n");
    printf("neurons : \n");
    for( u64 i = 0; i < layer->size; i++)
        printf("%lf ", layer->neurons[i]);
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
        printf("%lf ", layer->delta_neurons[i]);
    

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
        else if(strcmp(context->activation_functions[i], "lrelu")==0){
            neural_network->activation_function[i] = &apply_leaky_relu;
            neural_network->activation_d_function[i] = &apply_d_leaky_relu;
        }
        else {
            neural_network->activation_function[i] = &apply_sigmoid;
            neural_network->activation_d_function[i] = &apply_d_sigmoid;
        }
    }
    
}




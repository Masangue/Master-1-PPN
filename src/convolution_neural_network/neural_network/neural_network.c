#include "neural_network.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



////////////////////////////////////////////////////////
////          INTIT / FREE NN                       ////
////////////////////////////////////////////////////////


//  General free function, liberating all allocated memory to the NN
void free_neural_network(Neural_network * neural_network )
{
    for(u64 i = 0; i < neural_network->size ; i++){
        free_layer( &neural_network->layers[i] );
    }
    free( neural_network->layers );
    free( neural_network->expected );
    free( neural_network->activation_function );
    free( neural_network->activation_d_function );

    free( neural_network );
}


//  Creates and initializes the NN by calling previously defined functions
Neural_network * create_neural_network( Context * context ){
    
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
        create_layer( &neural_network->layers[i], topology[i], topology[i+1] );
    }

    for(u64 i = 0; i < nn_size - 1; i++){
        init_layer( &neural_network->layers[i]/*, topology[i+1], context->batch_size*/ );
    }
    
    prepare_activation( neural_network, context );

    free(topology);
    return neural_network;
}

//  Fills the input layers with the list of chars representing the image
void set_input_output(Neural_network * neural_network, u8 * input, int * output ){

    Layer * layer_input = &neural_network->layers[0];
    Layer * layer_output = &neural_network->layers[neural_network->size - 1];

    #pragma omp for
    for( u64 i = 0; i < layer_input->size; i++ ){
        layer_input->neurons[ i ] = (f64) input[i] / 255;
    }

    #pragma omp for
    for( u64 i = 0; i < layer_output->size; i++ ){
        neural_network->expected[i] = (f64) output[i] ;
    }

    return;
}

////////////////////////////////////////////////////////
////                STOCHASTIC                      ////
////////////////////////////////////////////////////////



//  Wrapper function, computing each layer forward
void stochastic_forward_compute( Neural_network * neural_network , Context * context ){
    for(u64 i = 0; i < neural_network->size - 1; i++){
        compute_layer( &neural_network->layers[i], &neural_network->layers[i+1], 0,
                      neural_network->activation_function[i+1] );
    }
}

//  Main function of the backpropagation process
//  Calls the three other backpropagation functions
void stochastic_backward_compute( Neural_network * neural_network, Context * context ){
    
    u64 nn_size = neural_network->size;

    compute_error_gradient( &neural_network->layers[nn_size - 1] , 
                         neural_network->expected );

    #pragma omp barrier

    for(u64 i = nn_size - 2; i > 0; i--){
        compute_gradient( &neural_network->layers[i], &neural_network->layers[i+1],
                         neural_network->activation_d_function[i] );
        
        #pragma omp barrier

        accumulate_gradient( &neural_network->layers[i], &neural_network->layers[i+1], 0 );

        #pragma omp barrier
    }

    for(u64 i = 0; i < nn_size - 1; i++){
        update_layer( &neural_network->layers[i], &neural_network->layers[i+1],
                      context->eta_, context->alpha_, 1 );
    }

}

////////////////////////////////////////////////////////
////                  BATCH FUNCTION                ////
////////////////////////////////////////////////////////



void batch_forward_propagation( Neural_network * neural_network, Context * context, u64 local_batch_iteration ){
   
    u64 nn_size = neural_network->size;

    for(u64 i = 0; i < nn_size - 1; i++){
        compute_layer( &neural_network->layers[i], &neural_network->layers[i+1], local_batch_iteration,
                    neural_network->activation_function[i+1] );
    }

    #pragma omp barrier

    compute_error_gradient( &neural_network->layers[nn_size - 1], neural_network->expected );
    
    #pragma omp barrier

    for(int i = nn_size - 2; i > 0; i--){
        compute_gradient( &neural_network->layers[i], &neural_network->layers[i+1],
                        neural_network->activation_d_function[i] );

        #pragma omp barrier


        accumulate_gradient( &neural_network->layers[i], &neural_network->layers[i+1], local_batch_iteration );

        #pragma omp barrier
    }
    accumulate_gradient( &neural_network->layers[0], &neural_network->layers[0+1], local_batch_iteration );

}

void batch_backward_propagation( Neural_network * neural_network, Context * context, u64 batch_size ){
    
    u64 nn_size = neural_network->size;

    //
    for(u64 i = 0; i < nn_size - 1; i++){
        update_layer( &neural_network->layers[i], &neural_network->layers[i+1],
                      context->eta_, context->alpha_, batch_size );
    }

    return;
}


////////////////////////////////////////////////////////
////                 ACTIVATION                     ////
////////////////////////////////////////////////////////



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
        else if(strcmp(context->activation_functions[i], "tanh")==0){
            neural_network->activation_function[i] = &apply_tanh;
            neural_network->activation_d_function[i] = &apply_d_tanh;
        }
        else {
            neural_network->activation_function[i] = &apply_sigmoid;
            neural_network->activation_d_function[i] = &apply_d_sigmoid;
        }
    }
}



////////////////////////////////////////////////////////
////               DEBUG / TOOLS                    ////
////////////////////////////////////////////////////////


//  Shuffles the dataset to prevents pattern redundancy
void shuffle(u64 size, u64 * tab){
    #pragma omp for
    for( u64 p = 0 ; p < size ; p++ ) { 
        tab[p] = p ;
    }
    #pragma omp for
    for( u64 p = 0 ; p < size - 1 ; p++) {
        u64 np = rand() % ( size - p) + p;
            
        u64 op = tab[p];

        #pragma omp critical
        {
            tab[p] = tab[np];
            tab[np] = op;
        }
    }
}


void    range(u64 size, u64 * tab){
    for( u64 p = 0 ; p < size ; p++ ) { 
        tab[p] = p ;
    }
}



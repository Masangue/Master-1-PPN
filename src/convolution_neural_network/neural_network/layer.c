#include "layer.h" 
#include <stdio.h>


////////////////////////////////////////////////////////
////          CREATION / INITIALISATION             ////
////////////////////////////////////////////////////////

//  Allocate a layer
void create_layer( Layer * layer, u64 size, u64 next_size ){
    layer->size = size;
    layer->next_size = next_size;

    layer->weights                      = aligned_alloc(64, size * next_size  * sizeof(f64) );
    layer->weights_gradient             = aligned_alloc(64, size * next_size  * sizeof(f64) );
    layer->weights_gradient_accumulator = aligned_alloc(64, size * next_size  * sizeof(f64) );
    layer->buffer = aligned_alloc(64, size * next_size  * sizeof(f64) );

    layer->bias                         = aligned_alloc(64, next_size *         sizeof(f64) );
    layer->bias_gradient                = aligned_alloc(64, next_size *         sizeof(f64) );
    layer->bias_gradient_accumulator    = aligned_alloc(64, next_size *         sizeof(f64) );

    layer->neurons       = aligned_alloc(64, size * sizeof(f64) );
    layer->delta_neurons = aligned_alloc(64, size * sizeof(f64) );


}
void free_layer( Layer * layer ){
    free( layer->weights );
    free( layer->weights_gradient );
    free( layer->weights_gradient_accumulator );

    free( layer->bias );
    free( layer->bias_gradient );
    free( layer->bias_gradient_accumulator );

    free( layer->buffer );

    free( layer->neurons );
    free( layer->delta_neurons );
}


f64 random_( f64 min, f64 max ){

    f64 rdm = ( (f64)rand() / (f64)RAND_MAX) * (max - min) + min ;
    return rdm;
}
//  Init a layer with random values
void init_layer( Layer * layer ){
    u64 size = layer->size;
    u64 next_size = layer->next_size;

    f64 range = sqrt(2.f / size );

    //
    for( u64 j = 0; j < next_size; j++ ){
        layer->bias[j]       =  random_(-range, range);
        layer->bias_gradient[j] = 0.0f;
        layer->bias_gradient_accumulator[j] = 0.0f;

        //
        for( u64 i = 0; i < size ; i++ ){
            layer->weights[j * size + i] = random_(-range, range);
            layer->weights_gradient[j * size + i] = 0.0f;
            layer->weights_gradient_accumulator[j * size + i] = 0.0f;
        }   
    }

    //
    for( u64 i = 0; i < size ; i++ ){
        layer->neurons[i]       = 0;
        layer->delta_neurons[i] = 0;

    }
}







////////////////////////////////////////////////////////
////             FORWARD PROPAGATION                ////
////////////////////////////////////////////////////////




//  Applies standard neural computation function
void compute_layer( Layer * layer1, Layer * layer2, u64 batch_iteration, activation_function_t * activation ){
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    
    f64 s = 0.0f;
    #pragma omp for
    for( u64 j = 0; j < next_size; j++ ){
        s = layer1->bias[j];
        for( u64 i = 0; i < size ; i++ ){
            s += layer1->neurons[ i ] * layer1->weights[ j * size + i ];
        }
        layer2->neurons[ j ] = s;
    }

    activation( layer2->neurons, layer2->neurons, next_size );  
}

////////////////////////////////////////////////////////
////             BACKWARD PROPAGATION                ////
////////////////////////////////////////////////////////



//  First step of the backpropagation process
//  Computes the output error. 
f64 compute_error_gradient( Layer * layer, f64 * expected ){
    u64 size = layer->size;
    
    #pragma omp for
    for( u64 i = 0; i < size; i++ ){
        layer->delta_neurons[ i ] =  layer->neurons[ i ] - expected[ i ]   ;
    }
    return 0;
}

void compute_gradient( Layer * layer1, Layer * layer2, activation_function_t * activation ){

    u64 size = layer1->size;
    u64 next_size = layer2->size;

    f64 s = 0.f;
    #pragma omp for
    for( u64 i = 0; i < size; i++ ){
        s = 0.0;
        for( u64 j = 0; j < next_size; j++ ){
            s += layer1->weights[j * size + i] * layer2->delta_neurons[ j ];
        }
 
        layer1->delta_neurons[ i ] = s ;
    }

    //delta_neurons = delta_neurons * d_sigmoid( neurons );
    activation( layer1->neurons,  layer1->delta_neurons, size );

}

void accumulate_gradient( Layer * layer1, Layer * layer2, u64 local_batch_counter ){
    
    u64 size = layer1->size;
    u64 next_size = layer2->size;

    f64 temp;

    if(local_batch_counter != 0){
        #pragma omp for
        for( u64 j = 0; j < next_size; j++){
            layer1->bias_gradient_accumulator[j] += layer2->delta_neurons[j];
            
            for( u64 i = 0; i < size; i++){
                temp = layer1->neurons[i] * layer2->delta_neurons[j];
                #pragma omp atomic 
                layer1->weights_gradient_accumulator[j * size + i] += temp;  
            }
        }
    }
    else{
        #pragma omp for
        for( u64 j = 0; j < next_size; j++){
            layer1->bias_gradient_accumulator[j] = layer2->delta_neurons[j];
        
            for( u64 i = 0; i < size; i++){
                temp = layer1->neurons[i] * layer2->delta_neurons[j];
                #pragma omp critical 
                {
                    layer1->weights_gradient_accumulator[j * size + i] = temp;        
                }
            }
        }
    }
}

f64 compute_L2_norm( Layer * layer ) {
    u64 size = layer->size;
    u64 next_size = layer->next_size;

    f64 acc = 0.0f;
    
    for( u64 j = 0; j < next_size; j++){

        //
        for( u64 i = 0; i < size; i++){

            acc += pow( layer->weights[ j * size + i],2 );
            // acc +=  fabs(layer->weights[ j * size + i]) ;
        }
    }

    return sqrt(acc);
};


//  Backpropagation process
//  Changes the weights of all neurons of a layer
void update_layer( Layer * layer1, Layer * layer2, f64 eta_, f64 alpha_, u64 batch_size ){
    
    u64 size = layer1->size;
    u64 next_size = layer2->size;
    // f64 regularization_step = 0.005 * 200 ;
    f64 regularization_step = 0.005 ;
    // f64 regularization_step = 0.0001;
    f64 L2_regularization = compute_L2_norm( layer1 );
    
    #pragma omp for
    for( u64 j = 0; j < next_size; j++){

        
        layer1->bias_gradient[j] =  (1.f/batch_size) * eta_ * layer1->bias_gradient_accumulator[j] 
                                        + alpha_ * layer1->bias_gradient[j];
        layer1->bias[j] -= layer1->bias_gradient[j];

        //
        for( u64 i = 0; i < size; i++){

            layer1->weights_gradient[j * size + i] =   (1.f / size ) * (1.f/batch_size) * eta_ * layer1->weights_gradient_accumulator[j * size + i] 
                                                     +  (  eta_ * regularization_step *   L2_regularization * (1.f/(size)))
                                                    + alpha_ * layer1->weights_gradient[j * size + i];
            layer1->weights[ j * size + i] -= layer1->weights_gradient[j * size + i];
        }
    }
}

// Debugging function
// Prints the whole NN in a visually clear format
void debug( Layer * layer ){
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
    //         printf("%lf ", layer->weights_gradient[ j * layer->size + i]);
    //     }
    // }

    printf("\n\n");
    printf("delta neurons : \n");
    for( u64 i = 0; i < layer->size; i++)
        printf("%lf ", layer->delta_neurons[i]);
    

}




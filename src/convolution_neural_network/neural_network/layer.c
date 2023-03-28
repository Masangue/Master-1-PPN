#include "layer.h" 


////////////////////////////////////////////////////////
////          CREATION / INITIALISATION             ////
////////////////////////////////////////////////////////

//  Allocate a layer
void create_layer( Layer * layer, u64 size, u64 next_size, u64 batch_size ){
    layer->size = size;
    layer->weights       = aligned_alloc(64, size * next_size  * sizeof(f64) );
    layer->bias          = aligned_alloc(64, next_size *         sizeof(f64) );

    layer->delta_weights = aligned_alloc(64, size * next_size  * sizeof(f64) );
    layer->delta_bias    = aligned_alloc(64, next_size *         sizeof(f64) );



    layer->neurons       = aligned_alloc(64, size * batch_size * sizeof(f64) );
    layer->delta_neurons = aligned_alloc(64, size * batch_size * sizeof(f64) );


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
    u64 offset1 = batch_iteration * size;
    u64 offset2 = batch_iteration * next_size;
    
    f64 s = 0.0f;
    for( u64 j = 0; j < next_size; j++ ){
        s = layer1->bias[j];
        for( u64 i = 0; i < size ; i++ ){
            s += layer1->neurons[offset1 + i] * layer1->weights[ j * size + i ];
        }
        // layer2->neurons[j] = sigmoid(s);
       layer2->neurons[offset2 + j] = s;
    }

    activation( layer2->neurons+offset2, layer2->neurons+offset2, next_size );
     
}

////////////////////////////////////////////////////////
////             BACKWARD PROPAGATION                ////
////////////////////////////////////////////////////////



//  First step of the backpropagation process
//  Computes the output error. 
f64 compute_output_delta( Layer * layer, f64 * expected, u64 batch_iteration ){
    u64 size = layer->size;
    u64 offset1 = batch_iteration * size;
    
    for( u64 i = 0; i < size; i++ ){
        layer->delta_neurons[offset1 + i] =  layer->neurons[offset1 + i] - expected[i]   ;
    }
    return 0;
}

//  Backpropagation process.
//  Computes the error delta for all neurons of a layer
//  This function will be called for each layer
void compute_delta( Layer * layer1, Layer * layer2, u64 batch_iteration, activation_function_t * activation ){

    u64 size = layer1->size;
    u64 next_size = layer2->size;
    u64 offset1 = batch_iteration * size;
    u64 offset2 = batch_iteration * next_size;

    f64 s = 0.f;
    for( u64 i = 0; i < size; i++ ){
        s = 0.0;
        for( u64 j = 0; j < next_size; j++ ){
            s += layer1->weights[j * size + i] * layer2->delta_neurons[offset2 + j];
        }
        // layer1->delta_neurons[i] = s * d_sigmoid( layer1->neurons[i] ) ;
 
        layer1->delta_neurons[offset1 + i] = s ;
    }

    //delta_neurons = delta_neurons * d_sigmoid( neurons );
    activation( layer1->neurons+offset1,  layer1->delta_neurons+offset1, size );

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
////                GRADIENT                        ////
////////////////////////////////////////////////////////


f64 get_weight_gradient( Layer * layer1, Layer * layer2, f64 batch_size, u64 i, u64 j ){

    u64 size = layer1->size;
    u64 next_size = layer2->size;
     
    f64 weight_gradient = 0;
    for(int k = 0; k < batch_size; k++){
        weight_gradient += layer1->neurons[i + k * size] * layer2->delta_neurons[ j + k * next_size ];
    }
    return weight_gradient = weight_gradient / batch_size;

}

f64 get_bias_gradient( Layer * layer2, f64 batch_size, u64 j ){

    u64 next_size = layer2->size;
     
    f64 bias_gradient = 0;
        for(int k = 0; k < batch_size; k++){
            bias_gradient += layer2->delta_neurons[ j + k * next_size ];
        }

    return bias_gradient = bias_gradient / batch_size;

}


////////////////////////////////////////////////////////
////                 DEBUG                          ////
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




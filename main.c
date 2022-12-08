/* 
This file contains a toy neural network that we will 
modify and improve to match the problem

This neural network is inspire from : 
https://www.cs.bham.ac.uk/~jxb/INC/nn.html
https://github.com/yaspr/nn

this NN will predict if the result of an addition (between two number between 0 and 10 ) is true.

i iterate through the Inputs
j iterate through the Hiddens
k iterate through the Outputs
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


// types
typedef double f64;
typedef unsigned int u32;
typedef unsigned long long u64;


// activations functions
f64 sigmoid(f64 x) {
    return 1 / (1 + exp(-x) );
}

f64 d_sigmoid(f64 x) {
    return x * (1 - x);
}



int main(int argc, char *argv[])
{
    printf(" id ; err\n");
    srand(time(NULL));
    
    f64 cumul_err = 0.0f;
    
    f64 eta = 0.5;
    f64 alpha = 0.3;
    // size
    u64 n_inputs  = 3;
    u64 n_hidden  = 6;
    u64 n_outputs = 1;

    f64 * neurons_inputs  = aligned_alloc(64, n_inputs * sizeof(f64) );
    f64 * neurons_hiddens = aligned_alloc(64, n_hidden * sizeof(f64) );
    f64 * neurons_outputs = aligned_alloc(64, n_outputs * sizeof(f64) );
    f64 * expected        = aligned_alloc(64, n_outputs * sizeof(f64) );
     
    f64 * weightsIH   = aligned_alloc(64, n_inputs  * n_hidden  *  sizeof(f64) );
    f64 * weightsHO   = aligned_alloc(64, n_hidden  * n_outputs *  sizeof(f64) );
    f64 * biasIH      = aligned_alloc(64, n_inputs  * n_hidden  *  sizeof(f64) );
    f64 * biasHO      = aligned_alloc(64, n_hidden  * n_outputs *  sizeof(f64) );
    
    // f64 * delta_inputs  = malloc( n_inputs * sizeof(f64) );
    f64 * delta_hiddens = aligned_alloc(64, n_hidden * sizeof(f64) );
    f64 * delta_outputs = aligned_alloc(64, n_outputs * sizeof(f64) );
    
    f64 * deltaWeightsIH   = aligned_alloc(64, n_inputs  * n_hidden  *  sizeof(f64) );
    f64 * deltaWeightsHO   = aligned_alloc(64, n_hidden  * n_outputs *  sizeof(f64) );
    f64 * deltaBiasIH      = aligned_alloc(64, n_inputs  * n_hidden  *  sizeof(f64) );
    f64 * deltaBiasHO      = aligned_alloc(64, n_hidden  * n_outputs *  sizeof(f64) );
    

    // f64 * bias_inputs  = malloc( n_inputs * sizeof(f64) );
    // f64 * bias_hiddens = malloc( n_hidden * sizeof(f64) );
    // f64 * bias_outputs = malloc( n_outputs * sizeof(f64) );
    
   
    f64 s = 0;

    // inititialisation with random weights (for now)
    for( u64 j = 0; j < n_hidden; j++ ){
        biasIH[j]      = ((f64) rand() / (f64)RAND_MAX) - 0.5;
        deltaBiasIH[j] = 0.0f;
        for( u64 i = 0; i < n_inputs ; i++ ){
            weightsIH[j * n_inputs + i] = ((f64) rand() / (f64)RAND_MAX) - 0.5;
            deltaWeightsIH[j * n_inputs + i] = 0.0f;
        }   
    }
    
    for( u64 k = 0; k < n_outputs; k++ ){
        biasHO[k]      = ((f64) rand() / (f64)RAND_MAX) - 0.5;
        deltaBiasHO[k] = 0.0f;
        for( u64 j = 0; j < n_hidden ; j++ ){
            weightsHO[k * n_hidden + j] = ((f64) rand() / (f64)RAND_MAX) - 0.5;
            deltaWeightsHO[k * n_hidden + j] = 0.0f;
        }   
    }
    
   //train
    u64 train_id = 0;
    while( train_id < 4000000){
        
        train_id ++;
        int a = rand() % 10 ; // random betwenn 0 and 10 
        int b = rand() % 10 ; // random betwenn 0 and 10 
        int c = 0.0;
        f64 err = 0.0; 

        
        // setting inputs and expected output
        if( ( (f64) rand() / (f64) RAND_MAX )  > 0.5 ){
            c = a + b;
            expected[0] = 1.0;
              
        }
        else{
            do {
                c = rand() % 19 ; // random betwenn 0 and 10 
                }
            while(c == a + b);
            expected[0] = 0.0;
        }
        
        // printf("a : %d\n", a);
        // printf("b : %d\n", b);
        // printf("c : %d\n", c);
        // printf("exepected : %f\n", expected);
        //
        
        //reduce each numbers to a value between -4 and 4, and use activation function on it.
        neurons_inputs[0] = sigmoid( ((f64) a - 4.5) / 2 );
        neurons_inputs[1] = sigmoid( ((f64) b - 4.5) / 2 );
        neurons_inputs[2] = sigmoid( ((f64) c - 9) / 4 );

        // printf("a : %f\n", neurons_inputs[0]);         
        // printf("b : %f\n", neurons_inputs[1]);         
        // printf("c : %f\n\n\n", neurons_inputs[2]);         
        //
        // getchar();

        // compute input -> hidden
        for( u64 j = 0; j < n_hidden; j++ ){
            s = biasIH[j];
            //fma
            for( u64 i = 0; i < n_inputs ; i++ ){
                s += neurons_inputs[i] * weightsIH[ j * n_inputs + i ];
            }
            neurons_hiddens[j] = sigmoid(s);
        }
        

        // compute hidden -> outputs  & errors 
        err = 0.0f;
        for( u64 k = 0; k < n_outputs; k++ ){
            s = biasHO[k];
            for( u64 j = 0; j < n_hidden; j++ ){
                s += neurons_hiddens[j] * weightsHO[ k * n_hidden + j ];
            }
            neurons_outputs[k] = sigmoid(s);

            //errors
            // Error += 0.5 * (Target[p][k] - Output[p][k]) * (Target[p][k] - Output[p][k]) ;   /* SSE */
            err += 0.5 * ( expected[k] - neurons_outputs[k] ) * ( expected[k] - neurons_outputs[k] );

            //delta outputs
            delta_outputs[k] = ( expected[k] - neurons_outputs[k] ) 
                                * d_sigmoid( neurons_outputs[k] );
        }

        cumul_err += err;

        //error
        if( train_id % 1000 == 0){
        // if( 1 == 1){
            printf(" %llu; %lf\n", train_id, cumul_err/1000);
            cumul_err = 0;
            // getchar();
        }

        // delta hidden
        // /!\ parcours par colonne /!\ //
        for( u64 j = 0; j < n_hidden; j++ ){
            s = 0.0;
            for( u64 k = 0; k < n_outputs; k++ ){
                s += weightsHO[k * n_hidden + j] * delta_outputs[k];
            }
            delta_hiddens[j] = s * d_sigmoid( neurons_hiddens[j] ) ;
        }

        // backpropagate        
        for( u64 j = 0; j < n_hidden; j++){
            deltaBiasIH[j] = eta * delta_hiddens[j] + alpha * deltaBiasIH[j];
            biasIH[j] += deltaBiasIH[j];
            for( u64 i = 0; i < n_inputs; i++){
                deltaWeightsIH[j * n_inputs + i] = eta * neurons_inputs[i] * delta_hiddens[j] + 
                                                   alpha * deltaWeightsIH[ j * n_inputs + i];
                weightsIH[ j * n_inputs + i] += deltaWeightsIH[j * n_inputs + i];
            }
        }
        
        for( u64 k = 0; k < n_outputs; k++){
            deltaBiasHO[k] = eta * delta_outputs[k] + alpha * deltaBiasHO[k];
            biasHO[k] += deltaBiasHO[k];
            for( u64 j = 0; j < n_hidden; j++){
                deltaWeightsHO[k * n_hidden + j] = eta * neurons_hiddens[j] * delta_outputs[k] + 
                                                   alpha * deltaWeightsHO[ k * n_hidden + j];
                weightsHO[ k * n_hidden + j] += deltaWeightsHO[k * n_hidden + j];
            }
        }
    
    }
     


    return 0;
}

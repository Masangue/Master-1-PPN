/* 
This file contains a toy neural network that we will 
modify and improve to match the problem

This neural network is inspire from : 
https://www.cs.bham.ac.uk/~jxb/INC/nn.html
https://github.com/yaspr/nn

this NN will predict if the result of an addition (between two number between 0 and 10 ) is true.

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

    // size
    u64 n_inputs  = 3;
    u64 n_hidden  = 6;
    u64 n_outputs = 1;

    f64 * neurons_inputs  = aligned_alloc(64, n_inputs * sizeof(f64) );
    f64 * neurons_hiddens = aligned_alloc(64, n_hidden * sizeof(f64) );
    f64 * neurons_outputs = aligned_alloc(64, n_outputs * sizeof(f64) );
    
    // f64 * delta_inputs  = malloc( n_inputs * sizeof(f64) );
    f64 * delta_hiddens = aligned_alloc(64, n_hidden * sizeof(f64) );
    f64 * delta_outputs = aligned_alloc(64, n_outputs * sizeof(f64) );
 

    // f64 * bias_inputs  = malloc( n_inputs * sizeof(f64) );
    // f64 * bias_hiddens = malloc( n_hidden * sizeof(f64) );
    // f64 * bias_outputs = malloc( n_outputs * sizeof(f64) );
    
    f64 * weightsIH   = aligned_alloc(64, n_inputs * n_hidden  *  sizeof(f64) );
    f64 * weightsHO   = aligned_alloc(64, n_hidden * n_outputs *  sizeof(f64) );
    
    f64 s = 0;

    // init random weights
    for( u64 i = 0; i < n_inputs; i++ ){
        for( u64 j = 0; j < n_hidden; j++ ){
            weightsIH[i * n_hidden + j] = (f64) rand() / (f64)RAND_MAX;
        }   
    }
    
    for( u64 i = 0; i < n_hidden; i++ ){
        for( u64 j = 0; j < n_outputs; j++ ){
            weightsIH[i * n_outputs + j] = (f64) rand() / (f64)RAND_MAX;
        }   
    }
    
   //train
    u64 train_id = 0;
    while( train_id < 4000000){
        
        train_id ++;
        int a = rand() % 10 ; // random betwenn 0 and 10 
        int b = rand() % 10 ; // random betwenn 0 and 10 
        int c = 0.0;
        f64 expected = 0.0;
        f64 err = 0.0; 

        
        // setting inputs and expected output
        if( ( (f64) rand() / (f64) RAND_MAX )  > 0.5 ){
            c = a + b;
            expected = 1.0;
              
        }
        else{
            do {
                c = rand() % 19 ; // random betwenn 0 and 10 
                }
            while(c == a + b);
            expected = 0.0;
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
        for( u64 i = 0; i < n_hidden; i++ ){
            
            s = 0.0;
            //fma
            for( u64 j = 0; j < n_inputs; j++ ){
                s += neurons_inputs[j] * weightsIH[ i * n_inputs + j ];
            }
            neurons_hiddens[i] = sigmoid(s);
        }
    
        // compute hidden -> outputs
        for( u64 i = 0; i < n_outputs; i++ ){
            
            s = 0.0;
            for( u64 j = 0; j < n_hidden; j++ ){
                s += neurons_hiddens[j] * weightsHO[ i * n_hidden + j ];
            }
            neurons_outputs[i] = sigmoid(s);
        }

        //error
        err = fabs( neurons_outputs[0] - expected );
        cumul_err += err;
        // if( train_id % 1000000 == 0){
        //     printf(" hello there\n");
        //     printf(" train_id : %llu; err : %lf\n", train_id, err);
        //     getchar();
        // }
       if( train_id % 1000 == 0){
            // printf(" hello there\n");
            // printf(" train_id : %llu; err : %lf\n", train_id, err);
            printf(" %llu; %lf\n", train_id, cumul_err/1000);
            cumul_err = 0;
            // getchar();
        }


        
        
        // delta output
        delta_outputs[0] = ( neurons_outputs[0] - expected ) * d_sigmoid( neurons_outputs[0] );
        
        // delta hidden
        f64 _c_ = delta_outputs[0];
        for( u64 i = 0; i < n_hidden; i++ ){
            delta_hiddens[i] = _c_ * weightsHO[ 0 * n_hidden + i] * d_sigmoid( neurons_hiddens[i] ) ;
        }

        // backpropagate        
        for( u64 i = 0; i < n_hidden; i++){
            for( u64 j = 0; j < n_inputs; j++){
                weightsIH[i * n_inputs + j] -= 0.5 * neurons_inputs[j] * delta_hiddens[i];
            }
        }

        for( u64 i = 0; i < n_hidden; i++){
            weightsHO[0 * n_hidden + i] -= 0.5 * neurons_hiddens[i] * delta_outputs[0];
        }
    
    }
     


    return 0;
}

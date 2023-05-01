
#include "activation.h"
#include <math.h>
#include <stdio.h>

//  Activations functions
f64 sigmoid(f64 x) {
    return 1 / (1 + exp(-x) );  }

f64 d_sigmoid(f64 x) {
    return x * (1 - x); }


f64 relu(f64 x) {
    if (x >= 0 && x <= 100) { return x; }
    else if (x >= 0 && x > 100) { return 100; }
    else  { return 0; }
}

f64 d_relu(f64 x) {
    if (x >= 0) { return 1; }
    else  { return 0; }
}


f64 leaky_relu(f64 x) {
    if (x >= 0) { return x; }
    else  { return x/10; }
}

f64 d_leaky_relu(f64 x) {
    if (x >= 0) { return 1; }
    else  { return 0.1; }
}


f64 tanh_(f64 x) {
    return  (exp(x) - exp(-x) ) / (exp(x) + exp(-x) );  
}

f64 d_tanh_(f64 x) {
    return 1 - pow(tanh_(x), 2);
}



//sig
void apply_d_sigmoid( f64 * src, f64 * dst, u64 size){
    for( u64 i = 0; i < size; i++ ){
        dst[i] *= d_sigmoid( src[i] ) ;
    }
}
void apply_sigmoid( f64 * src, f64 * dst, u64 size ){
    for( u64 i = 0; i < size; i++ ){
        dst[i] = sigmoid( src[i]);
    }
}

//relu
void apply_d_relu( f64 * src, f64 * dst, u64 size){
    for( u64 i = 0; i < size; i++ ){
        dst[i] *= d_relu( src[i] ) ;
    }
}
void apply_relu( f64 * src, f64 * dst, u64 size ){
    for( u64 i = 0; i < size; i++ ){
        dst[i] = relu( src[i]);
    }
}


//l_relu
void apply_d_leaky_relu( f64 * src, f64 * dst, u64 size){
    for( u64 i = 0; i < size; i++ ){
        dst[i] *= d_leaky_relu( src[i] ) ;
    }
}

void apply_leaky_relu( f64 * src, f64 * dst, u64 size ){
    for( u64 i = 0; i < size; i++ ){
        dst[i] = leaky_relu( src[i]);
    }
}


//l_relu
void apply_d_tanh( f64 * src, f64 * dst, u64 size){
    for( u64 i = 0; i < size; i++ ){
        dst[i] *= d_tanh_( src[i] ) ;
    }
}

void apply_tanh( f64 * src, f64 * dst, u64 size ){
    for( u64 i = 0; i < size; i++ ){
        dst[i] = tanh_( src[i]);
    }
}

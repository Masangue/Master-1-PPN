 #pragma once 
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "context.h"


// activations functions
f64     sigmoid(f64 x);
f64     d_sigmoid(f64 x);
f64     relu(f64 x);
f64     d_relu(f64 x);

void apply_d_sigmoid( f64 * src, f64 * dst, u64 size);
void apply_sigmoid( f64 * src, f64 * dst, u64 size );
void apply_d_relu( f64 * src, f64 * dst, u64 size);
void apply_relu( f64 * src, f64 * dst, u64 size );
void apply_leaky_relu( f64 * src, f64 * dst, u64 size );
void apply_d_leaky_relu( f64 * src, f64 * dst, u64 size );


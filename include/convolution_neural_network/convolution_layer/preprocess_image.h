 #pragma once
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "global.h"



#include "convolution_layer.h"

#include "dataset_manager.h" 
#include "context.h" 

int preprocess_dataset( Dataset * dataset, Context * context );

void convolution_function_from_context(Context * context, 
                                        convolution_function_t *** convo_funcs,
                                        u8 const*** convo_filters );

void  from_convolution_descriptor ( convolution_descriptor * descriptor, 
                                                        convolution_function_t ** func, 
                                                        const u8 ** filter);
 
size_t input_size_from_context(Context * context );

// convolution_function_t ** convolution_function_from_context(Context * context);
// convolution_function_t * from_convolution_descriptor ( convolution_descriptor * convo);

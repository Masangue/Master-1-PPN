#include "preprocess_image.h"
#include <stdio.h>
#include <stdlib.h>

int preprocess_dataset( Dataset * dataset, Context * context ){

    u8 *image_ptr = NULL;
    u8 *buffer_ptr = NULL;

    image_ptr  = malloc( IMAGE_SIZE * sizeof(unsigned char));
    buffer_ptr = malloc( IMAGE_SIZE * sizeof(unsigned char));

    convolution_function_t ** convo_funcs; 
    const u8 ** kernels; 

    convolution_function_from_context(context, &convo_funcs, &kernels);

    for( u64 i  = 0; i < dataset->size; i++ ){
        
        // load image data in image_ptr
        memcpy(image_ptr, dataset->images[i].pixels, sizeof(u8) *  
               dataset->images[i].width * dataset->images[i].height);

        dataset->images[i].inputs = apply_convolution_filters(
                image_ptr, buffer_ptr, 
                dataset->images[i].width, 
                dataset->images[i].height,
                convo_funcs, kernels, context->convo_size );
        free(dataset->images[i].pixels);
        dataset->images[i].pixels = NULL;
    }
    
    free(image_ptr);
    free(buffer_ptr);
    free(convo_funcs);
    free(kernels);


    return 0;
}

void  from_convolution_descriptor ( convolution_descriptor * descriptor, 
                                                        convolution_function_t ** func, 
                                                        const u8 ** kernel){
   
    // need test !

    //filter
    if(strcmp(descriptor->kernel, "basic")==0) 
        {
        if      (descriptor->size == 3 ) { *kernel = blur_3x3;}
        else if (descriptor->size == 5 ) { *kernel = blur_5x5;}
        else { printf("ERROR -1"); exit(0);}
        }
    else if(strcmp(descriptor->kernel, "none")==0)
        {*kernel = NULL;}
    else 
        {*kernel = NULL; printf("ERROR 0"); exit(0);}

    //function
    if(strcmp(descriptor->func, "convolution")==0) 
        {
        if      (descriptor->size == 3 ) { *func = &convolution_3X3;}
        else if (descriptor->size == 5 ) { *func = &convolution_5X5;}
        else { printf("ERROR 1"); exit(0);}
        }
    else if(strcmp(descriptor->func, "max_pool")==0)
        {
        if      (descriptor->size == 2 ) { *func = &max_pool_2X2; }
        // else if (descriptor->size == 3 ) { *func = &max_pool_3X3;}
        else { printf("ERROR 2"); exit(0);}
        }
    else if(strcmp(descriptor->func, "avg_pool")==0)
        {
        if      (descriptor->size == 2 ) { *func = &avg_pool_2X2;}
        else { printf("ERROR 3"); exit(0);}
        }
    else 
       { printf("ERROR 4"); exit(0);}

}

void convolution_function_from_context(Context * context, 
                                        convolution_function_t *** funcs,
                                        u8 const*** kernel ){
    int convo_size = context->convo_size;
    (*funcs) = malloc(convo_size * sizeof(convolution_function_t *) );
    (*kernel) = malloc(convo_size * sizeof(u8 *) );

    for (int i = 0; i < convo_size ; i++ ) {
        from_convolution_descriptor( &context->convo[i], &(*funcs)[i], &(*kernel)[i] );
    } 

}


size_t input_size_from_context(Context * context ){
    int convo_size = context->convo_size;
    int width = context->width;
    int height = context->height;

    for (int i = 0; i < convo_size ; i++ ) {
        convolution_descriptor * descriptor = &context->convo[i];
        if(strcmp(descriptor->func, "convolution")==0) 
            {
            if      (descriptor->size == 3 ) { height = height - 2;  width = width - 2;}
            else if (descriptor->size == 5 ) { height = height - 4;  width = width - 4;}
            else { printf("ERROR 1"); exit(0);}
            }
        else if(strcmp(descriptor->func, "max_pool")==0)
            {
            if      (descriptor->size == 2 ) { height = height / 2;  width = width / 2;}
            // else if (descriptor->size == 3 ) { *func = &max_pool_3X3;}
            else { printf("ERROR 2"); exit(0);}
            }
        else if(strcmp(descriptor->func, "avg_pool")==0)
            {
            if      (descriptor->size == 2 ) { height = height / 2;  width = width / 2;}
            else { printf("ERROR 3"); exit(0);}
            }
        else 
           { printf("ERROR 4"); exit(0);}

 
    }

    return (height) * (width);

}





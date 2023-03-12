#pragma once

// #include <inttypes.h>

// #include <spng.h>
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "type.h"
#include "global.h"
#include "image_io.h"


typedef void convolution_function_t( u8 ** image, u8 ** buffer, 
                                    size_t *height, size_t *width, 
                                    const u8 * kernel_filter );


// int * process_img(char *img);
unsigned char * apply_convolution_filters( u8 * image_ptr, u8 * buffer_ptr, 
                                          size_t image_width, size_t image_height, 
                                          convolution_function_t ** funcs, 
                                          const u8 ** kernel, u64 number_of_layer );


void convolution_5X5( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter);
void convolution_3X3( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter);
void max_pool_3X3   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * none);
void max_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * none);
void avg_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * none);




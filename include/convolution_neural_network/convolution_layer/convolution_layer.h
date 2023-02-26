#pragma once

// #include <inttypes.h>

// #include <spng.h>
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "type.h"
#include "global.h"



// int * process_img(char *img);
unsigned char * apply_convolution_filters( u8 * image_ptr, u8 * buffer_ptr, size_t image_width, size_t image_height );


void convolution_5X5( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter, int stride);
void convolution_3X3( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter, int stride);
void max_pool_3X3   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width );
void max_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width );
void avg_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width );




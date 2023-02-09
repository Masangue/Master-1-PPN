#pragma once
#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

#include <inttypes.h>
#include <stdio.h>

#include <spng.h>
#include "type.h"
#include <string.h> 
#include "type.h"
#include "global.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>



// int * process_img(char *img);
unsigned char * prepare_image( char * filename, u8 * image_ptr, u8 * buffer_ptr );
int process_img(char *img, unsigned char ** image, size_t * image_size, size_t * image_width, size_t * image_height  );

void convolution_5X5( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter, int stride);
void convolution_3X3( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter, int stride);
void max_pool_3X3   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width );
void max_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width );
void avg_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width );



#endif

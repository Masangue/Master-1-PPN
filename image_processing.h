#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

#include <spng.h>

#include <inttypes.h>
#include <stdio.h>

int * process_img(char *img);
void convolution_3X3(unsigned char * image, size_t height, size_t width, int * kernel_filter, int stride, int * conv_image);
void max_pool_3X3(int * conv_image, size_t height, size_t width, int * pool_image);
void max_pool_2X2_reduced_size(int * conv_image, size_t height, size_t width, int * pool_image);


#endif

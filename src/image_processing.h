#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H

#include <spng.h>

#include <inttypes.h>
#include <stdio.h>

// int * process_img(char *img);
unsigned char * prepare_image( char * filename );
int process_img(char *img, unsigned char ** image, size_t * image_size, size_t * image_width, size_t * image_height  );
void convolution_3X3(unsigned char * image, size_t height, size_t width, unsigned char * kernel_filter, int stride, unsigned char * conv_image);
void max_pool_3X3(unsigned char * conv_image, size_t height, size_t width, unsigned char * pool_image);
void max_pool_2X2_reduced_size(unsigned char * conv_image, size_t height, size_t width, unsigned char * pool_image);


#endif

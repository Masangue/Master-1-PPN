/*
This file contains the image processing for the neurals networks
All commented code has been left in case of potential future use

The use of libspng is inspired by : 
https://github.com/randy408/libspng/blob/v0.7.2/examples/example.c


*/ 

#include "convolution_layer.h"

i32 convolve_baseline( u8 *m, i32 *f, u64 fh, u64 fw, size_t height, size_t width ) {
    i32 r = 0;

    for (u64 i = 0; i < fh; i++)
        for (u64 j = 0; j < fw; j++){
            // printf("gx : %d\n",f[ i * fw + j ]);
            r += m[ i * width + j ] * f[ i * fw + j ];}

    return r;
}

//
void sobel_3X3( u8 ** image, u8 ** buffer, size_t *height, size_t *width, int threshold ) {
    f64 gx, gy;
    f32 mag = 0.0;

    i32 f1[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1}; // 3x3 matrix

    i32 f2[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1}; // 3x3 matrix

    //
    for (u64 i = 0; i < (*height - 3); i++) {
        for (u64 j = 0; j < (*width  - 3); j++) {
            // printf("gx : %d\n", (*image)[ i * (*width) + j ] );
            gx = convolve_baseline( &( (*image)[ i * (*width) + j ] ), f1, 3, 3, *height, *width );
            gy = convolve_baseline( &( (*image)[ i * (*width) + j ] ), f2, 3, 3, *height, *width );

            mag = sqrt((gx * gx) + (gy * gy));
            // printf("gx : %f\n",gy);
            // printf("gy : %f\n",gx);
            // printf("mag : %f\n",mag);
            (*buffer)[ i * (*width-2) + j ] = (mag > threshold) ? 255 : mag;
        }
    }
    // update size
    *height = *height - 2;
    *width  = *width  - 2;

    // swap buffer <=> image
    u8 * tmp;
    tmp = *image;
    *image = *buffer;
    *buffer = tmp;

}

/*  Convolution using a 3x3 kernel filter (unused at the moment) */
void convolution_3X3( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter, int stride) {
    for ( u64 i = 0; i < *height-2; i += stride) {
        for ( u64 j = 0; j < *width-2; j += stride) {
             
            u64 s = 0;
            for (size_t ik = 0; ik < 3; ik++) {
                for (size_t jk = 0; jk < 3; jk++) {
                    u64 filter_idx = ik * 3 + jk;
                    u64 image_idx = ( i + ik ) * ( *width ) + ( j + jk );
                    s += (*image)[ image_idx ] * kernel_filter[ filter_idx ];
                }
            }

            u64 buffer_idx = i * ( *width - 2 ) +  j;
            (*buffer)[buffer_idx] = (u8)( s / 5 );
        }
    }
    
    // update size
    *height = *height - 2;
    *width  = *width  - 2;

    // swap buffer <=> image
    u8 * tmp;
    tmp = *image;
    *image = *buffer;
    *buffer = tmp;
}


/*  Convolution using a 5x5 kernel filter */
void convolution_5X5( u8 ** image, u8 ** buffer, size_t *height, size_t *width, const u8 * kernel_filter, int stride) {

    
    for ( u64 i = 0; i < *height-4; i += stride) {
        for ( u64 j = 0; j < *width-4; j += stride) {
             
            u64 s = 0;
            for (size_t ik = 0; ik < 5; ik++) {
                for (size_t jk = 0; jk < 5; jk++) {
                    u64 filter_idx = ik * 5 + jk;
                    u64 image_idx = ( i + ik ) * ( *width ) + ( j + jk );
                    s += (*image)[ image_idx ] * kernel_filter[ filter_idx ];
                }
            }

            u64 buffer_idx = i * ( *width - 4 ) +  j;
            (*buffer)[buffer_idx] = (u8)( s / 9 );
        }
    }
    
    // update size
    *height = *height - 4;
    *width  = *width  - 4;

    // swap buffer <=> image
    u8 * tmp;
    tmp = *image;
    *image = *buffer;
    *buffer = tmp;
    
}

/*  Maxpool using a 3x3 sized tile (unused at the moment) */
// void max_pool_3X3(unsigned char * conv_image, size_t height, size_t width, unsigned char * pool_image) {
//     int max = 0;
//     for (size_t i = 1; i < height-1; i++) {
//         for (size_t j = 1; j < width-1; j++) {
//             max = 0;
//             for (int ik = -1; ik < 2; ik++) {
//                 for (int jk = -1; jk < 2; jk++) {
//                     if (conv_image[(i+ik)*width + (j+jk)] > max) {
//                         max = conv_image[(i+ik)*width + (j+jk)];
//                     }
//                 }
//             }
//             pool_image[(i-1)*(width-2) + (j-1)] = max;
//         }
//     }
// }

/*  Maxpool using a 2x2 sized tile */
void max_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width ){
    u8 max = 0;
    u8 val;
    for (u64 i = 0; i < *height - 1; i+=2) {
        for (u64 j = 0; j < *width - 1; j+=2) {

            max = 0;
            for (u64 ik = 0; ik < 2; ik++) {
                for (u64 jk = 0; jk < 2; jk++) {
                    val = (*image)[ (i+ik) * (*width) + (j+jk) ];
                    if ( val > max) 
                        max = val;
                }
            }

            (*buffer)[ (i/2) * ( (*width) / 2 ) + (j/2) ] = val;
        }
    }

    // update size
    *height = (*height) / 2;
    *width  = (*width) / 2;

    // swap buffer <=> image
    u8 * tmp;
    tmp = *image;
    *image = *buffer;
    *buffer = tmp;

}

/*  Avgpool using a 2x2 sized tile (unused at the moment) */
void avg_pool_2X2   ( u8 ** image, u8 ** buffer, size_t *height, size_t *width ){
    f64 avg = 0;
    for (u64 i = 0; i < *height - 1; i+=2) {
        for (u64 j = 0; j < *width - 1; j+=2) {

            avg = 0;
            for (u64 ik = 0; ik < 2; ik++) {
                for (u64 jk = 0; jk < 2; jk++) {
                    avg += (*image)[ (i+ik) * (*width) + (j+jk) ];
                }
            }
            (*buffer)[ (i/2) * ( (*width) / 2 ) + (j/2) ] = avg/4;

        }
    }
    // update size
    *height = (*height) / 2;
    *width  = (*width) / 2;

    // swap buffer <=> image
    u8 * tmp;
    tmp = *image;
    *image = *buffer;
    *buffer = tmp;

}


/*  Main image processing function, calling the functions previously
    defined in this file to process a given file to feed it to the NN
*/
unsigned char * apply_convolution_filters( u8 * image_ptr, u8 * buffer_ptr, size_t image_width, size_t image_height ) {

    size_t image_size = image_height * image_width;

    // open and decode the image
    // process_img(filename, &image_ptr, &image_size, &image_width, &image_height);
    

    convolution_5X5(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_5x5, 1 );
    max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
    convolution_5X5(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_5x5, 1 );
    max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
    max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
      
 //    convolution_3X3(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_3x3, 1 );
 //    sobel_3X3( &image_ptr, &buffer_ptr, &image_height, &image_width, 200 );
 //    avg_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
 //    convolution_5X5(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_5x5, 1 );
 //    max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
 //    convolution_3X3(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_3x3, 1 );

    // convolution_5X5(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_5x5, 1 );
    // avg_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
    // convolution_3X3(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_5x5, 1 );
    // convolution_5X5(&image_ptr, &buffer_ptr, &image_height, &image_width, blur_5x5, 1 );
  
    // max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
   
    // printf("%ld\n", image_width * image_height);
    // write_ppm("_ppm.ppm", image_ptr , image_width, image_height);
    // exit(0);
    u8 * inputs = NULL;
    inputs = aligned_alloc( 64, image_height * image_width * sizeof(u8) );
    memcpy(inputs, image_ptr, sizeof(u8) * image_height * image_width );

    return inputs;

    error2:
        printf("ERROR");
        exit(-1);
        return NULL;
}

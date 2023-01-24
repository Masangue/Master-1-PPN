/*
This file contains the image processing for the neurals networks
All commented code has been left in case of potential future use

The use of libspng is inspired by : 
https://github.com/randy408/libspng/blob/v0.7.2/examples/example.c


*/ 

#include "image_processing.h"
#include "type.h"
#include "global.h"
#include <stdlib.h>
#include <string.h>

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

/*  Creates a ppm image from tab, the currently processed image
    This function is used for debugging purposes only
    This code isn't ours, we used the source code located at this url :
    https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C  
*/
int write_ppm(char * filename, unsigned char * tab , size_t dimx, size_t dimy)
{
    // const int dimx = 800, dimy = 800;
    int i, j;
    FILE *fp = fopen(filename, "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", (int)dimx, (int)dimy);
    for (j = 0; j < dimy; ++j) {
        for (i = 0; i < dimx; ++i) {
            int index = j * dimx + i;
            static unsigned char color[3];
            color[0] = tab[index];  /* red */
            color[1] = tab[index];  /* green */
            color[2] = tab[index];  /* blue */
            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
    return EXIT_SUCCESS;
}

/*  Creates a greyscale version of a colored image
    By combining each pixel's value from a triple-value (0-255,0-255,0-255)
    to a single-value (0-255)
*/
// void rgb_to_grey( unsigned char * image, unsigned char * image_grayscale, size_t grayscale_size  ) {    
//     for (size_t i = 0; i < grayscale_size; i++) {
//         // classic method
//         image_grayscale[i] = (image[i*3] + image[i*3+1] + image[i*3+2])/3;
//
//         // weighted method
//         // image_grayscale[i] = (image[i*3]*0.2126 + image[i*3+1]*0.7152 + image[i*3+2]*0.0722);
//     }
// }

/* Converts a .png file to a pointer of chars using the libspng library */
int process_img(char *img, unsigned char ** image, size_t * image_size, size_t * image_width, size_t * image_height  ) 
{
    FILE *png;
    int ret = 0;
    spng_ctx *ctx = NULL;
    // unsigned char *image = NULL;
    // size_t image_size, image_width, image_height;

    png = fopen(img, "rb");

    if(png == NULL)
    {
        printf("error opening input file %s\n", img);
        goto error;
    }

    ctx = spng_ctx_new(0);

    if(ctx == NULL)
    {
        printf("spng_ctx_new() failed\n");
        goto error;
    }

    spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE);

    size_t limit = 1024 * 1024 * 64;
    spng_set_chunk_limits(ctx, limit, limit);

    spng_set_png_file(ctx, png);

    struct spng_ihdr ihdr;
    ret = spng_get_ihdr(ctx, &ihdr);

    if(ret)
    {
        printf("spng_get_ihdr() error: %s\n", spng_strerror(ret));
        goto error;
    }

    struct spng_plte plte = {0};
    ret = spng_get_plte(ctx, &plte);

    if(ret && ret != SPNG_ECHUNKAVAIL)
    {
        printf("spng_get_plte() error: %s\n", spng_strerror(ret));
        goto error;
    }

    if(!ret) printf("palette entries: %u\n", plte.n_entries);

    // size_t image_size, image_width, image_height;

    int fmt = SPNG_FMT_PNG;

    if(ihdr.color_type == SPNG_COLOR_TYPE_INDEXED) fmt = SPNG_FMT_RGB8;

    ret = spng_decoded_image_size(ctx, fmt, image_size);

    if(*image_size != IMAGE_SIZE){
        printf("ERROR : wrong image size");
        goto error;
    }

    if(ret) goto error;

    // *image = malloc(*image_size * sizeof(unsigned char));

    if(*image == NULL) goto error;

    ret = spng_decode_image(ctx, NULL, 0, fmt, SPNG_DECODE_PROGRESSIVE);

    if(ret)
    {
        printf("progressive spng_decode_image() error: %s\n", spng_strerror(ret));
        goto error;
    }

    *image_height = ihdr.height;
    *image_width = (*image_size) / (*image_height);

    struct spng_row_info row_info = {0};

    do
    {
        ret = spng_get_row_info(ctx, &row_info);
        if(ret) break;

        ret = spng_decode_row(ctx, (*image) + row_info.row_num * (*image_width), *image_width);
    }
    while(!ret);    

    if(ret != SPNG_EOI)
    {
        printf("progressive decode error: %s\n", spng_strerror(ret));

        if(ihdr.interlace_method)
            printf("last pass: %d, scanline: %u\n", row_info.pass, row_info.scanline_idx);
        else
            printf("last row: %u\n", row_info.row_num);
    }

    
    spng_ctx_free(ctx);
    fclose(png);

    return 0;

    error:
        spng_ctx_free(ctx);
        free(*image);
        fclose(png);
        return -1;

}

/*  Main image processing function, calling the functions previously
    defined in this file to process a given file to feed it to the NN
*/
unsigned char * prepare_image( char * filename, u8 * image_ptr, u8 * buffer_ptr ) {

    size_t image_size, image_width, image_height;
    //
    // u8 *image_ptr = NULL;
    // u8 *buffer_ptr = NULL;
    //
    // image_ptr = malloc( IMAGE_SIZE * sizeof(unsigned char));
    // buffer_ptr = malloc( IMAGE_SIZE * sizeof(unsigned char));
    //

    // open and decode the image
    process_img(filename, &image_ptr, &image_size, &image_width, &image_height);
    


      
    convolution_5X5(&image_ptr, &buffer_ptr, &image_height, &image_width, kernel_filter_5x5, 1 );
    max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
    convolution_5X5(&image_ptr, &buffer_ptr, &image_height, &image_width, kernel_filter_5x5, 1 );
    max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
    max_pool_2X2   (&image_ptr, &buffer_ptr, &image_height, &image_width );
    
    u8 * inputs = NULL;
    inputs = aligned_alloc( 64, image_height * image_width * sizeof(u8) );
    memcpy(inputs, image_ptr, sizeof(u8) * image_height * image_width );

    return inputs;

    error2:
        printf("ERROR");
        exit(-1);
        return NULL;
}

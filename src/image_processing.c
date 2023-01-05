/*
This file contains the image processing for the neurals networks


The use of libspng is inspire from : 
https://github.com/randy408/libspng/blob/v0.7.2/examples/example.c


*/ 

#include "image_processing.h"
#include "type.h"


void convolution_3X3(unsigned char * image, size_t height, size_t width, unsigned char * kernel_filter, /*size_t filter_height, size_t filter_width,*/ int stride, unsigned char * conv_image) {

    for (size_t i = 1; i < height-1; i += stride) {
        for (size_t j = 1; j < width-1; j += stride) {

            conv_image[(i-1)*(width-2) + (j-1)] = 1 

            + image[(i-1)*width + (j-1)] * kernel_filter[0] + image[(i-1)*width + (j  )] * kernel_filter[1] + image[(i-1)*width + (j+1)] * kernel_filter[2]

            + image[(i  )*width + (j-1)] * kernel_filter[3] + image[(i  )*width + (j  )] * kernel_filter[4] + image[(i  )*width + (j+1)] * kernel_filter[5]

            + image[(i+1)*width + (j-1)] * kernel_filter[6] + image[(i+1)*width + (j  )] * kernel_filter[7] + image[(i+1)*width + (j+1)] * kernel_filter[8]
            ;
        }
    }
}

void convolution_5X5(unsigned char * image, size_t height, size_t width, unsigned char * kernel_filter, /*size_t filter_height, size_t filter_width,*/ int stride, unsigned char * conv_image) {
    size_t filter_id = 0;
    for (size_t i = 0; i < height-4; i += stride) {
        for (size_t j = 0; j < width-4; j += stride) {

            conv_image[(i)*(width-4) + (j)] = 1;

            filter_id = 0;

            for (size_t ik = 0; ik < 5; ik++) {
                for (size_t jk = 0; jk < 5; jk++) {
                    conv_image[(i)*(width-4) + (j)] += image[(i+ik)*width + (j+jk)] * kernel_filter[filter_id];
                    filter_id += 1;
                }
            }

            conv_image[(i)*(width-4) + (j)] = conv_image[(i)*(width-4) + (j)]/13;
        }
    }
}

void max_pool_3X3(unsigned char * conv_image, size_t height, size_t width, unsigned char * pool_image) {
    int max = 0;
    for (size_t i = 1; i < height-1; i++) {
        for (size_t j = 1; j < width-1; j++) {
            max = 0;
            for (int ik = -1; ik < 2; ik++) {
                for (int jk = -1; jk < 2; jk++) {
                    if (conv_image[(i+ik)*width + (j+jk)] > max) {
                        max = conv_image[(i+ik)*width + (j+jk)];
                    }
                }
            }
            pool_image[(i-1)*(width-2) + (j-1)] = max;
        }
    }
}

void max_pool_2X2_reduced_size(unsigned char * conv_image, size_t height, size_t width, unsigned char * pool_image) {
    int max = 0;
    for (size_t i = 0; i < height-1; i+=2) {
        for (size_t j = 0; j < width-1; j+=2) {
            max = 0;
            for (int ik = 0; ik < 2; ik++) {
                for (int jk = 0; jk < 2; jk++) {
                    if (conv_image[(i+ik)*width + (j+jk)] > max) {
                        max = conv_image[(i+ik)*width + (j+jk)];
                    }
                }
            }
            pool_image[(i/2)*(width/2) + (j/2)] = max;
        }
    }
}

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

void rgb_to_grey( unsigned char * image, unsigned char * image_grayscale, size_t grayscale_size  ) {    
    for (size_t i = 0; i < grayscale_size; i++) {
        // classic method
        image_grayscale[i] = (image[i*3] + image[i*3+1] + image[i*3+2])/3;

        // weighted method
        // image_grayscale[i] = (image[i*3]*0.2126 + image[i*3+1]*0.7152 + image[i*3+2]*0.0722);
    }
}

int process_img(char *img, unsigned char ** image, size_t * image_size, size_t * image_width, size_t * image_height  ) {
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

    if(ret) goto error;

    *image = malloc(*image_size);

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

unsigned char * prepare_image( char * filename ) {

    unsigned char *image = NULL;
    size_t image_size, image_width, image_height;

    // open and decode the image
    process_img(filename, &image, &image_size, &image_width, &image_height);



    //convert image to grayscale (if needed)

    // unsigned char * image_grayscale = malloc(image_size/3 * sizeof(unsigned char));

    // if(image_grayscale == NULL) goto error2;
    
    // image_size = image_size/3;
    // image_width = image_width/3;
    // image_height = image_height;

    // rgb_to_grey( image, image_grayscale, image_size );


    


    /*  first convolution with a kernel filter :
            1 0 1
            0 1 0
            1 0 1
    */

    // int * image_conv = malloc((image_width-2) * (image_height-2) * sizeof(int));
    // if(image_conv == NULL) goto error2;

    // image_width = image_width-2;
    // image_height = image_height-2;
    // image_size = image_width*image_height;


    // int * kernel_filter_3x3 = malloc(9 * sizeof(int));
    // if(kernel_filter_3x3 == NULL) goto error2;

    // kernel_filter_3x3[0] = 1, kernel_filter_3x3[2] = 1, kernel_filter_3x3[4] = 1, kernel_filter_3x3[6] = 1, kernel_filter_3x3[8] = 1;
    // kernel_filter_3x3[1] = 0, kernel_filter_3x3[3] = 0, kernel_filter_3x3[5] = 0, kernel_filter_3x3[7] = 0;

       
    // convolution_3X3(image, image_height, image_width, kernel_filter_3x3, 1, image_conv);



    /*  first convolution with a kernel filter :
            1 0 1 0 1
            0 1 0 1 0
            1 0 1 0 1
            0 1 0 1 0
            1 0 1 0 1
    */

    
    unsigned char * image_conv = malloc((image_width-4) * (image_height-4) * sizeof(unsigned char));
    if(image_conv == NULL) goto error2;

    image_width = image_width-4;
    image_height = image_height-4;
    image_size = image_width*image_height;


    unsigned char * kernel_filter_5x5 = malloc(5*5 * sizeof(unsigned char));
    if(kernel_filter_5x5 == NULL) goto error2;


    for (size_t i = 0; i < 5*5; i++) {
        if (i%2 == 0) {
            kernel_filter_5x5[i] = 1;
        }
        else {
            kernel_filter_5x5[i] = 0;
        }   
    }
       
    convolution_5X5(image, image_height, image_width, kernel_filter_5x5, 1, image_conv);
    
    unsigned char * image_pool = malloc((image_width/2) * (image_height/2) * sizeof(unsigned char));
    if(image_pool == NULL) goto error2;


    // first max pool

    max_pool_2X2_reduced_size(image_conv, image_height, image_width, image_pool);

    image_width = image_width/2;
    image_height = image_height/2;
    image_size = image_width*image_height;


    /*  second convolution with a kernel filter :
            1 0 1 0 1
            0 1 0 1 0
            1 0 1 0 1
            0 1 0 1 0
            1 0 1 0 1
    */

    unsigned char * image_conv_2 = malloc((image_width-4) * (image_height-4) * sizeof(unsigned char));
    if(image_conv_2 == NULL) goto error2;

    convolution_5X5(image_pool, image_height, image_width, kernel_filter_5x5, 1, image_conv_2);

    image_width = image_width-4;
    image_height = image_height-4;
    image_size = image_width*image_height;


    // second max pool

    unsigned char * image_pool_2 = malloc((image_width/2) * (image_height/2) * sizeof(unsigned char));
    if(image_pool_2 == NULL) goto error2;

    max_pool_2X2_reduced_size(image_conv_2, image_height, image_width, image_pool_2);

    image_width = image_width/2;
    image_height = image_height/2;
    image_size = image_width*image_height;


    //apercu de l'image
    //write_ppm("test_img", image_pool_2, image_width, image_height);
   

    free(image);
    // free(image_grayscale);
    free(image_conv);
    free(image_pool);
    free(image_conv_2);
    // free(kernel_filter_3x3);
    free(kernel_filter_5x5);
    return image_pool_2;

    error2:
        free(image);
        // free(image_grayscale);
        free(image_conv);
        free(image_pool);
        free(image_conv_2);
        free(image_pool_2);
        // free(kernel_filter_3x3);
        free(kernel_filter_5x5);
        return NULL;
}
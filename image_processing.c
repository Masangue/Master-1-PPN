/*
This file contains the image processing for the neurals networks


The use of libspng is inspire from : 
https://github.com/randy408/libspng/blob/v0.7.2/examples/example.c


*/ 

#include <spng.h>

#include <inttypes.h>
#include <stdio.h>

void convolution_3X3(unsigned char * image, size_t height, size_t width, int * kernel_filter, /*size_t filter_height, size_t filter_width,*/ int stride, int * conv_image) {

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

void max_pool_3X3(int * conv_image, size_t height, size_t width, int * pool_image) {
    printf("\n");
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

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("no input file\n");
        return 1;
    }

    // for each images
    for (size_t img_iter = 1 ; img_iter < argc ; img_iter++) {

        printf("\nImage %s :\n", argv[img_iter]);

        FILE *png;
        int ret = 0;
        spng_ctx *ctx = NULL;
        unsigned char *image = NULL;

        png = fopen(argv[img_iter], "rb");

        if(png == NULL)
        {
            printf("error opening input file %s\n", argv[img_iter]);
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

        size_t image_size, image_width, image_height;

        int fmt = SPNG_FMT_PNG;

        if(ihdr.color_type == SPNG_COLOR_TYPE_INDEXED) fmt = SPNG_FMT_RGB8;

        ret = spng_decoded_image_size(ctx, fmt, &image_size);

        if(ret) goto error;

        image = malloc(image_size);

        if(image == NULL) goto error;

        ret = spng_decode_image(ctx, NULL, 0, fmt, SPNG_DECODE_PROGRESSIVE);

        if(ret)
        {
            printf("progressive spng_decode_image() error: %s\n", spng_strerror(ret));
            goto error;
        }

        image_height = ihdr.height;
        image_width = image_size / image_height;

        struct spng_row_info row_info = {0};

        do
        {
            ret = spng_get_row_info(ctx, &row_info);
            if(ret) break;

            ret = spng_decode_row(ctx, image + row_info.row_num * image_width, image_width);
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


        // convert image to grayscale

        unsigned char * image_grayscale = malloc(image_size/3 * sizeof(unsigned char));
        
        size_t grayscale_size = image_size/3;
        size_t grayscale_width = image_width/3;
        size_t grayscale_height = image_height;

        for (size_t i = 0; i < grayscale_size; i++) {
            // classic method
            image_grayscale[i] = (image[i*3] + image[i*3+1] + image[i*3+2])/3;

            // weighted method
            // image_grayscale[i] = (image[i*3]*0.2126 + image[i*3+1]*0.7152 + image[i*3+2]*0.0722);
        }

    
        for (size_t i = 0; i < grayscale_size; i++) {
            if (i%(grayscale_width) == 0) {
                printf("\n");
            }
            printf("%3d ", image_grayscale[i]);
        }
        printf("\n");



         
        /*  first convolution with a kernel filter :
             1 0 1
             0 1 0
             1 0 1
        */

        int * image_conv = malloc((grayscale_width-2) * (grayscale_height-2) * sizeof(int));

        int * kernel_filter = malloc(9 * sizeof(int));

        kernel_filter[0] = 1, kernel_filter[2] = 1, kernel_filter[4] = 1, kernel_filter[6] = 1, kernel_filter[8] = 1;
        kernel_filter[1] = 0, kernel_filter[3] = 0, kernel_filter[5] = 0, kernel_filter[7] = 0;

        printf("\nkernel_filter : \n");
        for (size_t i = 0; i < 9 ; i++) {
            if (i%(3) == 0) {
                printf("\n");
            }
            printf("%3d ", kernel_filter[i]);
        }
        printf("\n");

        convolution_3X3(image_grayscale, grayscale_height, grayscale_width, kernel_filter, 1, image_conv);

        printf("\nConvolution : \n");
        for (size_t i = 0; i < (grayscale_width-2) * (grayscale_height-2); i++) {
            if (i%(grayscale_width-2) == 0) {
                printf("\n");
            }
            printf("%4d ", image_conv[i]);
        }
        printf("\n");

        int * image_pool = malloc((grayscale_width-2-2) * (grayscale_height-2-2) * sizeof(int));

        // Max pooling 3X3

        max_pool_3X3(image_conv, (grayscale_height-2), (grayscale_width-2), image_pool);
        
        printf("\nMax pool : \n");
        for (size_t i = 0; i < (grayscale_width-2-2) * (grayscale_height-2-2); i++) {
            if (i%(grayscale_width-2-2) == 0) {
                printf("\n");
            }
            printf("%4d ", image_pool[i]);
        }
        printf("\n");

        error:
            spng_ctx_free(ctx);
            free(image);
            free(image_grayscale);
            free(image_conv);
            free(image_pool);
            free(kernel_filter);
            fclose(png);

    }
    
    return 0;
}
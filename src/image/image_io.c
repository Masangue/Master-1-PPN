#include "image_io.h"


int store_image_ppm(char * filename, unsigned char * tab , size_t dimx, size_t dimy)
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

int load_image(char *img, unsigned char ** image, size_t * image_width, size_t * image_height  ) 
{
    FILE *png;
    int ret = 0;
    spng_ctx *ctx = NULL;
    size_t image_size_p;
    size_t * image_size = &image_size_p;
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

// print an ascii art of a 20*24 images
// this function will be remove or move
void display_ascii_image(u8 * inputs, int width, int height){
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            int val = inputs[h * width + w];
            char s;
            if( val < 20)
                s = ' ';
            else if ( val < 50)
                s = '.';
            else if ( val < 100)
                s = ':';
            else if ( val < 150)
                s = 'o';
            else 
                s = '%';

            printf("%c%c%c",s,s,s);
        }
    printf("\n"); 
    }
    printf("\n\n"); 
}

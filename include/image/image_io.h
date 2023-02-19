#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <spng.h>
#include <inttypes.h>
// #include <math.h>


#include "type.h"
#include "global.h"



int store_image_ppm(char * filename, unsigned char * tab , size_t dimx, size_t dimy);

int load_image(char *img, unsigned char ** image, size_t * image_size, size_t * image_width, size_t * image_height  );


int store_image(char *img, unsigned char ** image, size_t * image_size, size_t * image_width, size_t * image_height  );



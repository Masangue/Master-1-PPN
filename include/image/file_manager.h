#pragma once

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "type.h"
#include "image_processing.h"
#include "global.h"



/* mri_image data struct used to manipulate dataset */
typedef struct {
    char * filename; // Name of current image
    int value ; // Value from 0 to 3, ranging from NonDemented to ModerateDemented
    u8 * inputs; // Result of image processing, which will be fed to the input layer of the NN
} mri_image;


int load_dataset( char * dirs[], int dir_num, mri_image * dataset, int max_per_folder);
int count_file(char * foldername);

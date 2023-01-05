 #pragma once

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef struct {
    char * filename;
    int value ;
    unsigned char * inputs;
} mri_image;


int load_dataset( char * dirs[], int dir_num, mri_image * dataset, int max_per_folder);
int count_file(char * foldername);

#pragma once 
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <libconfig.h>

#include "type.h"
#include "global.h"

#define STRING_SIZE 50

typedef struct {
    char * context_path;

    // ouput
    char * storage_dir;
    char * train_dat_path;
    char * test_dat_path;

    // dataset
    int max_per_folder;
    char ** train_dirs;
    char ** test_dirs;

    // nn
    int * topology;
    int nn_size;

    // training
    int do_test;
    int max_epoch;
    double precision;
    double alpha_;
    double eta_;
 
    //
    int width;
    int height;
    int filters; // TODO

} Context;

int load_context( Context * context, char * filename);
int info_context( Context * context );
int free_context( Context * context );


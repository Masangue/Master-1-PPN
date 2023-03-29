#pragma once 
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include <libconfig.h>

#include "type.h"
#include "global.h"


typedef struct {
    char * func;
    int size;
    char * kernel;
} convolution_descriptor ;

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
    char ** activation_functions;

    // training
    int batch_size;
    int do_test;
    int max_epoch;
    double precision;
    double alpha_;
    double eta_;
 
    // convolution
    int width;
    int height;
    int convo_size;
    convolution_descriptor * convo;

} Context;

int load_context( Context * context, char * filename);
int info_context( Context * context );
int free_context( Context * context );

int mpi_share_context( Context * context );
int mpi_rcv_context  ( Context * context );
int mpi_send_context ( Context * context );


#pragma once

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "neural_network.h"

typedef struct {
    double f1;
    double precision;
    double recall;
    double accuracy;
    double specificity;

    int true_positive;
    int false_positive;
    int true_negative;
    int false_negative;

} Score;


void init_score( Score * score );
int mpi_sync_score( Score * score, Mpi_neural_network_context * mpi_nn_context );
void update_score( Layer * output_layer, f64 * expected, Score * score );
void process_score( Score * score );

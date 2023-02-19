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


void initScore( Score * score );
void updateScore( Layer * output_layer, f64 * expected, Score * score );
void processScore( Score * score );

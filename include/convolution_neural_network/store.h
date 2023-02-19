#pragma once
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "neural_network.h"

void store_nn( char * path, Layer ** nn, u64 nb_layers, u64 * neurons_per_layers );
Layer ** load_nn( char * path, u64 nb_layers, u64 * neurons_per_layers );

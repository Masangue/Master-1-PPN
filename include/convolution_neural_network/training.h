#pragma once
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "global.h"

#include "neural_network.h"

#include "evaluation.h" 
#include "dataset_manager.h" 


int train(u64 max_epoch, f64 precision, Dataset * train_dataset, 
          Dataset * test_dataset, Layer ** neural_network, 
          u64 nb_layers, FILE * fp_train, FILE * fp_test ) ;

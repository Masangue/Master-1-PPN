#pragma once
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "global.h"

#include "neural_network.h"
#include "convolution_layer.h"

#include "evaluation.h" 
#include "dataset_manager.h" 
#include "context.h" 


int train(Context * context, Dataset * train_dataset, 
          Dataset * test_dataset, Layer ** neural_network ) ;

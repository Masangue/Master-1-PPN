#pragma once
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "global.h"

#include "convolution_layer.h"
#include "preprocess_image.h"

#include "neural_network.h"

#include "evaluation.h" 
#include "dataset_manager.h" 
#include "context.h" 


int train(Context * context, Dataset * train_dataset, 
          Dataset * test_dataset, Neural_network * neural_network ) ;

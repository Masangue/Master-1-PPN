#pragma once
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "neural_network.h"
#include "context.h"

void store_neural_network( Context * context, Neural_network * neural_network );
Neural_network * load_neural_network( Context * context  );

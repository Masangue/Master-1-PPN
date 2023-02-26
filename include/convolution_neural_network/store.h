#pragma once
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#include "type.h"
#include "neural_network.h"
#include "context.h"

void store_neural_network( Context * context, Layer ** layers );
Layer ** load_neural_network( Context * context  );

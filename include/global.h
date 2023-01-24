#pragma once

#include "type.h"

// image size :
#define IMAGE_WIDTH  176
#define IMAGE_HEIGHT 208
#define IMAGE_SIZE   36608


// kernel filters 
static const u8 kernel_filter_5x5 [25] = {
  1, 0 , 0 , 0 , 1 ,
  0, 1 , 0 , 1 , 0 ,
  0, 0 , 1 , 0 , 0 ,
  0, 1 , 0 , 1 , 0 ,
  1, 0 , 0 , 0 , 1 
};



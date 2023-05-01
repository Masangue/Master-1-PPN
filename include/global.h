#pragma once

#include "type.h"

// image size :
#define IMAGE_WIDTH  176
#define IMAGE_HEIGHT 208
#define IMAGE_SIZE   36608

#define ROOT 0
#define MASTER_RANK 0
#define STRING_SIZE 50


// kernel filters 
static const u8 blur_5x5 [25] = {
  1, 0 , 0 , 0 , 1 ,
  0, 1 , 0 , 1 , 0 ,
  0, 0 , 1 , 0 , 0 ,
  0, 1 , 0 , 1 , 0 ,
  1, 0 , 0 , 0 , 1 
};

static const u8 blur_3x3 [9] = {
  1 , 0 , 1 ,
  0 , 1 , 0 ,
  1 , 0 , 1 
};


static const u8 sobel_3x3 [9] = {
  1 , 2 , 1 ,
  0 , 1 , 0 ,
  1 , -2 , 1 
};



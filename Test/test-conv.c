#include <stdint.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <cmocka.h>

#include "image_processing.h"

// Doesnt work
static void test_convolutions(void **state){

    unsigned char img[25] =
    {
        2, 0,88, 0, 2,
        0, 2, 0, 2, 0,
        1, 0, 2, 0, 1,
        0, 2, 0, 2, 0,
        2, 0, 8, 0, 2
    };


    unsigned char kernel_five_1[25] =
    {
        1, 0, 0, 0, 1,
        0, 1, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 1, 0,
        1, 0, 0, 0, 1
    };

    unsigned char kernel_five_2[25] =
    {
        2, 0, 0, 0, 2,
        0, 2, 0, 2, 0,
        0, 0, 2, 0, 0,
        0, 2, 0, 2, 0,
        2, 0, 0, 0, 2
    };


    unsigned char *res = malloc( sizeof( unsigned char ) );

    // check with the kernel_five_1
    convolution_5X5( img, 5, 5, kernel_five_1, 1, res );
    assert_int_equal(2, res[0]);

    // check with the kernel_five_2
    convolution_5X5( img, 5, 5, kernel_five_2, 1, res );
    assert_int_equal(4, res[0]);

    free(res);
}

int main(void) {
  int result = 0;
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_convolutions),
  };
  result |= cmocka_run_group_tests_name("convolution", tests, NULL, NULL);

  return result;
}

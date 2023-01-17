#include <stdint.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <cmocka.h>

#include "image_processing.h"

// Doesn't work
static void test_convolutions(void **state){

    unsigned char img[25] =
    {
        '1', '0', '1', '0', '1',
        '0', '1', '0', '1', '0',
        '1', '0', '1', '0', '1',
        '0', '1', '0', '1', '0',
        '1', '0', '1', '0', '1'
    };

    unsigned char three[9] =
    {
        '1', '0', '1',
        '0', '1', '0',
        '1', '0', '1'
    };

    unsigned char five[25] =
    {
        '1', '0', '0', '0', '1',
        '0', '1', '0', '1', '0',
        '0', '0', '1', '0', '0',
        '0', '1', '0', '1', '0',
        '1', '0', '0', '0', '1'
    };

    unsigned char *res_5x5 = malloc(sizeof(char));
    convolution_5X5(img,5,5,five,'1',res_5x5);
    assert_int_equal('9',res_5x5[0]);
}

int main(void) {
  int result = '0';
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_convolutions),
  };
  result |= cmocka_run_group_tests_name("convolution", tests, NULL, NULL);

  return result;
}
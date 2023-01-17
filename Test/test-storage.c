#include <stdint.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <cmocka.h>

#include "type.h"
#include "nn.h"
#include "store.h"

static void test_storage(void **state){
    u64 nb_layers = 4;
    u64 neurons_per_layers[10] = {10,20,3,1,1};
    u64 input_size = neurons_per_layers[0];

    Layer ** nn1 = Init_Neural_network(neurons_per_layers, nb_layers);
    
    store_nn("storage", nn1, nb_layers, neurons_per_layers);
 
    Layer ** nn2 = load_nn("storage", nb_layers, neurons_per_layers);
    
    float eps = 0.001;
    assert_float_equal(nn1[0]->weights[0], nn2[0]->weights[0],eps);
    assert_float_equal(nn1[1]->weights[0], nn2[1]->weights[0],eps);
    assert_float_equal(nn1[1]->weights[5], nn2[1]->weights[5],eps);

    free_all(nn1, nb_layers);
    free_all(nn2, nb_layers);
}

int main(void) {
  int result = 0;
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_storage),
  };
  result |= cmocka_run_group_tests_name("storage", tests, NULL, NULL);

  return result;
}

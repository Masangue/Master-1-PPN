#include <stdint.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <cmocka.h>

#include "type.h"
#include "nn.h"

static void test_nn(void **state){
    
    Layer * layer1 = createLayer(10, 10);
    Layer * layer2 = createLayer(10, 10);

    u64 size = 10;
    u64 next_size = 10;

    // fill
    for( u64 j = 0; j < next_size; j++ ){
        layer1->bias[j]       =  1;
        layer2->bias[j]       =  0;
        for( u64 i = 0; i < size ; i++ ){
            layer1->weights[j * size + i] = 0;
            layer2->weights[j * size + i] = 0;
        }   
    }

    for( u64 i = 0; i < size ; i++ ){
        layer1->neurons[i]       = 10;
        layer2->neurons[i]       = 0;

    }

    computeLayer(layer1, layer2);
    float eps = 0.001;
    assert_float_equal(sigmoid(1), layer2->neurons[0] ,eps);



    // fill
    for( u64 j = 0; j < next_size; j++ ){
        layer1->bias[j]       =  0;
        layer2->bias[j]       =  0;
        for( u64 i = 0; i < size ; i++ ){
            layer1->weights[j * size + i] = 0;
            layer2->weights[j * size + i] = 0;
        }   
    }

    for( u64 i = 0; i < size ; i++ ){
        layer1->neurons[i]       = 10;
        layer2->neurons[i]       = 0;

    }
    computeLayer(layer1, layer2);
    assert_float_equal(sigmoid(0), layer2->neurons[0] ,eps);


    // free
    free(layer1->bias);
    free(layer1->neurons);
    free(layer1->weights);

    free(layer1->delta_neurons);
    free(layer1->delta_weights);
    free(layer1->delta_bias);


    free(layer2->bias);
    free(layer2->neurons);
    free(layer2->weights);

    free(layer2->delta_neurons);
    free(layer2->delta_weights);
    free(layer2->delta_bias);

}


static void test_sigmoid(void **state){

    assert_in_range(sigmoid(0), 0, 1);
    assert_in_range(sigmoid(1), 0, 1);
    assert_in_range(sigmoid(999), 0, 1);
    assert_in_range(sigmoid(-999), 0, 1);
    assert_in_range(sigmoid(123), 0, 1);
}

int main(void) {
  int result = 0;
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_nn),
      cmocka_unit_test(test_sigmoid),
  };
  result |= cmocka_run_group_tests_name("nn", tests, NULL, NULL);
  result |= cmocka_run_group_tests_name("sigmoid", tests, NULL, NULL);

  return result;
}

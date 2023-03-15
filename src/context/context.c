#include "context.h"
#include <stdio.h>
#include <stdlib.h>

//https://github.com/hyperrealm/libconfig/blob/master/examples/c/example1.c

int load_context( Context * context, char * filename){
    
    config_t cfg;
    config_setting_t *setting;
    config_init( &cfg );
    
    const char * buffer;

    context->context_path = filename;

    if(! config_read_file( &cfg, filename)) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
      config_destroy(&cfg);
      return(EXIT_FAILURE);
    }
    
    //output 
    context->storage_dir = malloc( STRING_SIZE * sizeof(char) );
    context->train_dat_path = malloc( STRING_SIZE * sizeof(char) );
    context->test_dat_path = malloc( STRING_SIZE * sizeof(char) );

    config_lookup_string(&cfg, "output.storage",   &buffer );
    strncpy( context->storage_dir, buffer, STRING_SIZE - 1 );

    config_lookup_string(&cfg, "output.train_dat", &buffer );
    strncpy( context->train_dat_path, buffer, STRING_SIZE - 1 );

    config_lookup_string(&cfg, "output.test_dat",  &buffer );
    strncpy( context->test_dat_path, buffer, STRING_SIZE - 1 );
   


    //dataset
    config_lookup_int(&cfg, "dataset.max_per_folder",  &context->max_per_folder );
        //Train
    setting = config_lookup ( &cfg, "dataset.train_dirs");
    context->train_dirs = malloc( 2 * sizeof(char *) );
    for( int i = 0; i < 2; i++ ){
        buffer = config_setting_get_string_elem( setting, i);
        context->train_dirs[i] = malloc( STRING_SIZE * sizeof(char) );
        strncpy( context->train_dirs[i], buffer, STRING_SIZE - 1 );
    }

        //Test
    setting = config_lookup ( &cfg, "dataset.test_dirs");
    context->test_dirs = malloc( 2 * sizeof(char *) );
    for( int i = 0; i < 2; i++ ){
        buffer = config_setting_get_string_elem( setting, i);
        context->test_dirs[i] = malloc( STRING_SIZE * sizeof(char) );
        strcpy( context->test_dirs[i], buffer );
    }

    

    //nn
    setting = config_lookup ( &cfg, "nn.topology");
    context->nn_size = config_setting_length ( setting ) + 2;
    context->topology = malloc( context->nn_size * sizeof(int) );
    for( int i = 0; i < context->nn_size - 2; i++ ){
        context->topology[i] = config_setting_get_int_elem( setting, i);
    }

    //nn activation function
    setting = config_lookup ( &cfg, "nn.activation");
    context->activation_functions = malloc( context->nn_size * sizeof(char *) );
    for( int i = 0; i < context->nn_size; i++ ){
        buffer = config_setting_get_string_elem( setting, i);
        context->activation_functions[i] = malloc( STRING_SIZE * sizeof(char) );
        strcpy( context->activation_functions[i], buffer );
    }



    //training
    config_lookup_int(&cfg, "training.batch_size",  &context->batch_size );
    config_lookup_int(&cfg, "training.do_test",  &context->do_test );
    config_lookup_int(&cfg, "training.max_epoch",  &context->max_epoch );
    config_lookup_float(&cfg, "training.precision",  &context->precision );
    config_lookup_float(&cfg, "training.alpha",  &context->alpha_ );
    config_lookup_float(&cfg, "training.eta",  &context->eta_ );


    // image processing

    config_lookup_int(&cfg, "image_processing.width",  &context->width );
    config_lookup_int(&cfg, "image_processing.height",  &context->height );

    setting = config_lookup(&cfg, "image_processing.filters");
    int convo_size = config_setting_length(setting);
    context->convo_size = convo_size;
    context->convo = malloc(convo_size * sizeof(convolution_descriptor) );

    for(int i = 0; i < convo_size; ++i )
    {
        context->convo[i].func = malloc( STRING_SIZE * sizeof(char) );
        context->convo[i].kernel = malloc( STRING_SIZE * sizeof(char) );

        config_setting_t *filters = config_setting_get_elem(setting, i);

        const char *func, *kernel;
        int size;

        config_setting_lookup_string(filters, "func", &func);
        config_setting_lookup_int(filters, "size", &size);
        config_setting_lookup_string(filters, "kernel", &kernel);

        strcpy( context->convo[i].func, func );
        strcpy( context->convo[i].kernel, kernel );
        context->convo[i].size = size;
    }


    config_destroy( &cfg );
    return 0;
}



int info_context( Context * context ){
    
    printf(" Context loaded with '%s' \n", context->context_path );

    printf("\n\n");
    printf("Train dirs : \n");
    for( int i = 0; i < 2; i++ ){
        printf("-> '%s' \n", context->train_dirs[i] );
    }

    printf("\n");
    printf("Test dirs : \n");
    for( int i = 0; i < 2; i++ ){
        printf("-> '%s' \n", context->test_dirs[i] );
    }


    printf("\n");
    printf("storage dirs : '%s' \n", context->storage_dir);
    printf("train dat dirs : '%s' \n", context->train_dat_path);
    printf("test  dat dirs : '%s' \n", context->test_dat_path);


    printf("\n");
    printf(" NN topology: \n ");
    for( int i = 0; i < context->nn_size-2; i++ ){
        printf(" %d ", context->topology[i] );
    }
     printf("\n");
    printf(" NN activations: \n ");
    for( int i = 0; i < context->nn_size; i++ ){
        printf(" %s ", context->activation_functions[i] );
    }
    printf("\nnn size : %d \n", context->nn_size);


    printf("\n");
    printf("max per folder : %d \n", context->max_per_folder);
    printf("batch size : %d \n", context->batch_size);
    printf("do test : %d \n", context->do_test);
    printf("max epoch : %d \n", context->max_epoch);
    printf("precision : %f \n", context->precision);
    printf("alpha : %f \n", context->alpha_);
    printf("eta : %f \n", context->eta_);


    printf("\n");
    for(int i = 0; i < context->convo_size; ++i )
    {
        printf("\t %s_%dx%d %s \n", context->convo[i].func, context->convo[i].size,  context->convo[i].size, context->convo[i].kernel  );
    }

    printf("width : %d \n", context->width);
    printf("height : %d \n", context->height);


    return 0;
}


int free_context( Context * context ){
    for( int i = 0; i < 2; i++ ){
        free(context->test_dirs[i]);
    }
    free(context->test_dirs);

    for( int i = 0; i < 2; i++ ){
        free(context->train_dirs[i]);
    }
    free(context->train_dirs);
    // free(context->context_path);
    free(context->storage_dir);
    free(context->train_dat_path);
    free(context->test_dat_path);
    free(context->topology);


    
    return 0;

}

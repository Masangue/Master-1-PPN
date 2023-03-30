#include "context.h"
#include <stdio.h>

//https://github.com/hyperrealm/libconfig/blob/master/examples/c/example1.c
int create_context( Context * context ){

    context->context_path = malloc( STRING_SIZE * sizeof(char) );

    context->storage_dir = malloc( STRING_SIZE * sizeof(char) );
    context->train_dat_path = malloc( STRING_SIZE * sizeof(char) );
    context->test_dat_path = malloc( STRING_SIZE * sizeof(char) );
    
    context->train_dirs = malloc( 2 * sizeof(char*) );
    context->test_dirs = malloc( 2 * sizeof(char*) );
    for (int j = 0; j < 2; j++){
        context->train_dirs[j] = malloc( STRING_SIZE * sizeof(char) );
        context->test_dirs[j] = malloc( STRING_SIZE * sizeof(char) );
    }


    context->topology = malloc( MAX_NN_SIZE * sizeof(int) );
    context->activation_functions = malloc( MAX_NN_SIZE * sizeof(char*) );
    for (int j = 0; j < MAX_NN_SIZE; j++){
        context->activation_functions[j] = malloc( STRING_SIZE * sizeof(char) );
    }


    context->convo = malloc( MAX_NN_SIZE * sizeof(convolution_descriptor) );
    for(int j = 0; j < MAX_NN_SIZE; ++j ){
        context->convo[j].func = malloc( STRING_SIZE * sizeof(char) );
        context->convo[j].kernel = malloc( STRING_SIZE * sizeof(char) );
    }

    return 0;

}

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
    for( int i = 0; i < 2; i++ ){
        buffer = config_setting_get_string_elem( setting, i);
        // context->train_dirs[i] = malloc( STRING_SIZE * sizeof(char) );
        strncpy( context->train_dirs[i], buffer, STRING_SIZE - 1 );
    }

        //Test
    setting = config_lookup ( &cfg, "dataset.test_dirs");
    for( int i = 0; i < 2; i++ ){
        buffer = config_setting_get_string_elem( setting, i);
        strcpy( context->test_dirs[i], buffer );
    }

    

    //nn
    setting = config_lookup ( &cfg, "nn.topology");
    context->nn_size = config_setting_length ( setting ) + 2;
    for( int i = 0; i < context->nn_size - 2; i++ ){
        context->topology[i] = config_setting_get_int_elem( setting, i);
    }

    //nn activation function
    setting = config_lookup ( &cfg, "nn.activation");
    for( int i = 0; i < context->nn_size; i++ ){
        buffer = config_setting_get_string_elem( setting, i);
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

    for(int i = 0; i < convo_size; ++i )
    {

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

    for( int i = 0; i < context->nn_size; i++ ){
        free(context->activation_functions[i]);
    }
    free(context->activation_functions);
    

    for(int i = 0; i < context->convo_size; ++i )
    {
        free( context->convo[i].func );
        free( context->convo[i].kernel );
    }
    free(context->convo);


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



int mpi_send_context( Context * context ){
    
    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
   
    // MPI_Status sta;

    for (int i = 0; i < P; i++) {
        if ( i == MASTER_RANK ) 
            continue;



        MPI_Ssend( context->context_path, STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
        
       
        // output
        MPI_Ssend( context->storage_dir, STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend( context->train_dat_path, STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend( context->test_dat_path, STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );

        //dataset
        MPI_Ssend(&context->max_per_folder, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        for (int j = 0; j < 2; j++){
            MPI_Ssend( context->train_dirs[j], STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
            MPI_Ssend( context->test_dirs[j], STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
        }
 
        //nn
        MPI_Ssend(&context->nn_size,  1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend( context->topology, context->nn_size - 2, MPI_INT, i, 1000, MPI_COMM_WORLD );

        for (int j = 0; j < context->nn_size; j++){
            MPI_Ssend( context->activation_functions[j], STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
        }


        // training
        MPI_Ssend(&context->batch_size, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(&context->do_test, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(&context->max_epoch, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(&context->precision, 1, MPI_DOUBLE, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(&context->alpha_, 1, MPI_DOUBLE, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(&context->eta_, 1, MPI_DOUBLE, i, 1000, MPI_COMM_WORLD );

        // convolution
        MPI_Ssend(&context->width, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(&context->height, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        MPI_Ssend(&context->convo_size, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
        for(int j = 0; j < context->convo_size; ++j ){
            MPI_Ssend(&context->convo[j].size,  1, MPI_INT, i, 1000, MPI_COMM_WORLD );
            MPI_Ssend(context->convo[j].func,   STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
            MPI_Ssend(context->convo[j].kernel, STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );
        }

    }

    return 0;
}

int mpi_recv_context  ( Context * context ){

    
    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    
    printf("P%d receiving from P%d\n", rank, MASTER_RANK);

    MPI_Status sta;

    MPI_Recv( context->context_path, STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta );
       
    // output
    MPI_Recv( context->storage_dir, STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv( context->train_dat_path, STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv( context->test_dat_path, STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );

    //dataset
    MPI_Recv(&context->max_per_folder, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    for (int j = 0; j < 2; j++){
        MPI_Recv( context->train_dirs[j], STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        MPI_Recv( context->test_dirs[j], STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    }
    
    //nn
    MPI_Recv(&context->nn_size,  1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv( context->topology, context->nn_size - 2, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );

    for (int j = 0; j < context->nn_size; j++){
        MPI_Recv( context->activation_functions[j], STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    }


    // training
    MPI_Recv(&context->batch_size, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv(&context->do_test, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv(&context->max_epoch, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv(&context->precision, 1, MPI_DOUBLE, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv(&context->alpha_, 1, MPI_DOUBLE, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv(&context->eta_, 1, MPI_DOUBLE, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );

    // convolution
    MPI_Recv(&context->width, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv(&context->height, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    MPI_Recv(&context->convo_size, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    
    for(int j = 0; j < context->convo_size; ++j ){
        MPI_Recv(&context->convo[j].size,  1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        MPI_Recv(context->convo[j].func,   STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        MPI_Recv(context->convo[j].kernel, STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
    }



    return 0;
}

int mpi_share_context( Context * context ){
    
    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if ( P == 1 ) {
        return 0;
    }

    if ( rank == MASTER_RANK )
        mpi_send_context( context );
        // printf("do nothing\n");
    else
        mpi_recv_context( context );

    return 0; 
}



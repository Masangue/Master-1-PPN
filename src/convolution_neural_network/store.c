#include "store.h" 

/* Stores a trained NN in a file, in order to be loaded for test */
void store_neural_network( Context * context, Neural_network * neural_network ){
    char sbuf[1024];

    Layer * layers = neural_network->layers;

    for(int i = 0; i < neural_network->size - 1; i++){
        // Layer * layer = nn[i];
        u64 next_size = context->topology[i+1];
        u64 size = context->topology[i];

        const char * path = context->storage_dir;
        // 
        // //
        // sprintf (sbuf, "%s/%d%s", path, i,"bias.dat");
        // FILE *bias = fopen(sbuf,"w");
        //
        // for( u64 j = 0; j < next_size; j++ ){
        //     fprintf(bias,"%lf\n", layer->bias[j]);
        //     // printf("print : %lf\n", layer->bias[j]);
        // }
        //




        
            




        //
        sprintf (sbuf, "%s/%d%s", path, i,"bias.dat");
        FILE *bias = fopen(sbuf,"w");
        for( u64 j = 0; j < next_size; j++ ){
            fprintf(bias, "%lf\n", layers[i].bias[j]);
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"delta_bias.dat");
        FILE *delta_bias = fopen(sbuf,"w");
        for( u64 j = 0; j < next_size; j++ ){
            fprintf(delta_bias, "%lf\n", layers[i].delta_bias[j]);
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"weight.dat");
        FILE *weight = fopen(sbuf,"w");
        for( u64 j = 0; j < next_size; j++ ){
            for( u64 ii = 0; ii < size ; ii++ ){
                fprintf(weight, "%lf\n", layers[i].weights[j * size + ii]);
            }
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"delta_weight.dat");
        FILE *delta_weight = fopen(sbuf,"w");
        for( u64 j = 0; j < next_size; j++ ){
            for( u64 ii = 0; ii < size ; ii++ ){
                fprintf(delta_weight, "%lf\n", layers[i].delta_weights[j * size + ii]);
            }
        }
//
        sprintf (sbuf, "%s/%d%s", path, i,"neurons.dat");
        FILE *neurons = fopen(sbuf,"w");
        for( u64 j = 0; j < size; j++ ){
            fprintf(neurons, "%lf\n", layers[i].neurons[j]);
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"delta_neurons.dat");
        FILE *delta_neurons = fopen(sbuf,"w");
        for( u64 j = 0; j < size; j++ ){
            fprintf(delta_neurons, "%lf\n", layers[i].delta_neurons[j]);
        }




        fclose(bias);
        fclose(delta_bias);

        fclose(weight);
        fclose(delta_weight);

        fclose(neurons);
        fclose(delta_neurons);


    }   

}

/* Loads a trained NN from a file, in order to test it */
Neural_network * load_neural_network( Context * context  ){

    Neural_network * neural_network = init_neural_network( context );
    int nn_size = neural_network->size;
    Layer * layers = neural_network->layers;


    char sbuf[1024];
    // char test[1024];

    const char * path = context->storage_dir;

    for(int i = 0; i < nn_size - 1; i++){
        u64 next_size = context->topology[i+1];
        u64 size = context->topology[i];

        f64 value = 0;
         

//
        sprintf (sbuf, "%s/%d%s", path, i,"bias.dat");
        FILE *bias = fopen(sbuf,"r");
        for( u64 j = 0; j < next_size; j++ ){
            int sc = fscanf(bias, "%lf\n", &value);
            // printf("id value : %lld %lf\n", j, value);
            layers[i].bias[j] = value;
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"delta_bias.dat");
        FILE *delta_bias = fopen(sbuf,"r");
        for( u64 j = 0; j < next_size; j++ ){
            int sc = fscanf(delta_bias, "%lf\n", &value);
            // printf("id value : %lld %lf\n", j, value);
            layers[i].delta_bias[j] = value;
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"weight.dat");
        FILE *weight = fopen(sbuf,"r");
        for( u64 j = 0; j < next_size; j++ ){
            for( u64 ii = 0; ii < size ; ii++ ){
                int sc = fscanf(weight, "%lf\n", &value);
                // printf("id value : %lld %lf\n", j, value);
                layers[i].weights[j * size + ii] = value;
            }
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"delta_weight.dat");
        FILE *delta_weight = fopen(sbuf,"r");
        for( u64 j = 0; j < next_size; j++ ){
            for( u64 ii = 0; ii < size ; ii++ ){
                int sc = fscanf(delta_weight, "%lf\n", &value);
                // printf("id value : %lld %lf\n", j, value);
                layers[i].delta_weights[j * size + ii] = value;
            }
        }
//
        sprintf (sbuf, "%s/%d%s", path, i,"neurons.dat");
        FILE *neurons = fopen(sbuf,"r");
        for( u64 j = 0; j < size; j++ ){
            int sc = fscanf(neurons, "%lf\n", &value);
            // printf("id value : %lld %lf\n", j, value);
            layers[i].neurons[j] = value;
        }

//
        sprintf (sbuf, "%s/%d%s", path, i,"delta_neurons.dat");
        FILE *delta_neurons = fopen(sbuf,"r");
        for( u64 j = 0; j < size; j++ ){
            int sc = fscanf(delta_neurons, "%lf\n", &value);
            // printf("id value : %lld %lf\n", j, value);
            layers[i].delta_neurons[j] = value;
        }




        fclose(bias);
        fclose(delta_bias);

        fclose(weight);
        fclose(delta_weight);

        fclose(neurons);
        fclose(delta_neurons);




    }


    return neural_network;

}

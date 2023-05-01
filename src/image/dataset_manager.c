#include "dataset_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mpi_send_dataset( Dataset * dataset ){

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
   
    // MPI_Status sta;
    for (int i = 0; i < P; i++) {
        if ( i == MASTER_RANK ) 
            continue;

        u64 dataset_size = dataset->size;
        MPI_Ssend( &dataset_size, 1, MPI_U64, i, 1000, MPI_COMM_WORLD );
        for (u64 j = 0; j < dataset_size; j++) {
            
            // u64 size_image = dataset->images[j].original_width * dataset->images[j].original_height; 
           
            // printf("send %lu\n", dataset->images[j].width );
            MPI_Ssend( &dataset->images[j].width, 1, MPI_U64, i, 1000, MPI_COMM_WORLD );
            MPI_Ssend( &dataset->images[j].height, 1, MPI_U64, i, 1000, MPI_COMM_WORLD );

            MPI_Ssend( &dataset->images[j].original_width, 1, MPI_U64, i, 1000, MPI_COMM_WORLD );
            MPI_Ssend( &dataset->images[j].original_height, 1, MPI_U64, i, 1000, MPI_COMM_WORLD );
            
            MPI_Ssend( &dataset->images[j].value, 1, MPI_INT, i, 1000, MPI_COMM_WORLD );
            MPI_Ssend( dataset->images[j].filename, STRING_SIZE, MPI_CHAR, i, 1000, MPI_COMM_WORLD );

            MPI_Ssend( dataset->images[j].pixels, IMAGE_SIZE, MPI_U8, i, 1000, MPI_COMM_WORLD );

        }

    }
 
    return 0;
}


int mpi_recv_dataset( Dataset * dataset ){

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
   
    MPI_Status sta;

    MPI_Recv( &dataset->size, 1, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta );
    u64 dataset_size = dataset->size;
    for (u64 j = 0; j < dataset_size; j++) {
        
        dataset->images[j] = create_mri_image();
        
        MPI_Recv( &dataset->images[j].width, 1, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        MPI_Recv( &dataset->images[j].height, 1, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );

        MPI_Recv( &dataset->images[j].original_width, 1, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        MPI_Recv( &dataset->images[j].original_height, 1, MPI_U64, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        
        MPI_Recv( &dataset->images[j].value, 1, MPI_INT, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );


        MPI_Recv( dataset->images[j].filename, STRING_SIZE, MPI_CHAR, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );
        MPI_Recv( dataset->images[j].pixels, IMAGE_SIZE, MPI_U8, MASTER_RANK, 1000, MPI_COMM_WORLD, &sta  );

    }

    
 
    return 0;
}

int mpi_share_dataset( Dataset * dataset ){

    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if ( P == 1 ) {
        return 0;
    }

    if ( rank == MASTER_RANK )
        mpi_send_dataset( dataset );
        // printf("do nothing\n");
    else{
        mpi_recv_dataset( dataset );
    }

    return 0; 
}

mri_image create_mri_image(){
    mri_image img;
    img.pixels = malloc( IMAGE_SIZE * sizeof(unsigned char));
    img.filename = malloc( STRING_SIZE * sizeof( char ));
    img.inputs = NULL;
    return img;
 
};

Dataset create_dataset( Context * context ){
   
    Dataset dataset;

    int num_folder = 2;
    int max_per_folder = context->max_per_folder;
    int max_size = num_folder * max_per_folder;  

    dataset.size = 0;  
    dataset.images = malloc( max_size * sizeof(mri_image) );

    return dataset;
}




/*  Dataset loader. 
    The function reads the data directory file by file, granting them a value from 0 to 3
    (possible optimization : increase the number of files treated at the same time)
    and sends each file to the prepare_image function for image processing*/
int init_dataset( char ** dirs, Dataset * dataset, Context * context ){

    int num_folder = 2;
    int max_file_per_folder = context->max_per_folder;
    dataset->size = 0;

    for( int i = 0; i < num_folder; i++ ){

        char ** files_name = NULL;
        char * folder_name = dirs[i];

        int nb_files_in_current_directory = get_filemame_from_dir( folder_name  , &files_name, max_file_per_folder );

        for (int j = 0; j < nb_files_in_current_directory ; j++) {
            int image_number = dataset->size;
            
            dataset->images[image_number] = create_mri_image();
   
            char image_path[512];
            snprintf(image_path, sizeof( image_path ), "%s/%s", folder_name, files_name[j]);
 
            load_image( image_path, &dataset->images[image_number].pixels, 
                        &dataset->images[image_number].original_width, 
                        &dataset->images[image_number].original_height);
            dataset->images[image_number].value = i;

            strcpy( dataset->images[image_number].filename, files_name[j] );
            dataset->images[image_number].width  = dataset->images[image_number].original_width;
            dataset->images[image_number].height = dataset->images[image_number].original_height;
            dataset->size ++; 

            free(files_name[j]);
        }
        free(files_name);
    }
    printf("Dataset filled with %lld images\n", dataset->size );
    return 0;
}


int free_mri_image( mri_image * image ){
    free( image->filename );
    if ( image->inputs != NULL ) {
        free( image->inputs );

    }
    if ( image->pixels != NULL ) {
        free( image->pixels );
    }

    return 0;
}

int free_dataset( Dataset * dataset ){

    for( u64 i = 0; i < dataset->size; i++ ){
         free_mri_image( &dataset->images[i] ); 
    }  

    free( dataset->images );
    return 0;
}



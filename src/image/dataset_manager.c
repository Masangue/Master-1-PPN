#include "dataset_manager.h"
#include <stdio.h>
#include <stdlib.h>

Dataset create_dataset( Context * context ){
   
    Dataset dataset;

    int num_folder = 2;
    int max_per_folder = context->max_per_folder;


    dataset.size = num_folder * max_per_folder;  
    dataset.images = malloc( dataset.size * sizeof(mri_image) );

    return dataset;
}


/*  File counter used in function main */
int count_file_in_directory(char * foldername){
    int counter = 0;
    
    DIR *d;
    struct dirent *dir;
    d = opendir(foldername);
    if (d) {
        while ((dir = readdir(d)) != NULL ) {
            if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
                continue;
            }
            counter++;
        }
        closedir(d);
    }

    return counter;
 
}

/*  Dataset loader. 
    The function reads the data directory file by file, granting them a value from 0 to 3
    (possible optimization : increase the number of files treated at the same time)
    and sends each file to the prepare_image function for image processing*/
int init_dataset( char ** dirs, Dataset * dataset, Context * context ){

    int num_folder = 2;
    int max_per_folder = context->max_per_folder;


    int folder_counter = 0;
    int total_counter = 0;

    for( int i = 0; i < num_folder; i++ ){
        DIR *d;
        struct dirent *dir;
        d = opendir(dirs[i]);
        if (d) {
            while ((dir = readdir(d)) != NULL && !(folder_counter >= max_per_folder) ) {
                
                if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
                    continue;
                }

                char image_path[512];
                snprintf(image_path, sizeof( image_path ), "%s/%s", dirs[i], dir->d_name);

                
                dataset->images[total_counter].pixels = malloc( IMAGE_SIZE * sizeof(unsigned char));
                load_image( image_path, &dataset->images[total_counter].pixels, 
                            &dataset->images[total_counter].original_width, 
                            &dataset->images[total_counter].original_height);
                dataset->images[total_counter].value = i;
                dataset->images[total_counter].filename = "name";
                dataset->images[total_counter].width  = dataset->images[total_counter].original_width;
                dataset->images[total_counter].height = dataset->images[total_counter].original_height;
                // printf("\n%lu x %lu \n",dataset->images[total_counter].width, dataset->images[total_counter].height );


                total_counter++;
                folder_counter++; 
            }
            folder_counter = 0; 
            closedir(d);
        }
        else{
            printf("%s does not exist", dirs[i]);
            exit(0);
        }
    }
    //update dataset size to its true value
    dataset->size = total_counter; 

    printf("Dataset filled with %d images\n", total_counter );
    return total_counter;
}



int free_mri_image( mri_image * image ){
    // free( image->filename );
    free( image->inputs );
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



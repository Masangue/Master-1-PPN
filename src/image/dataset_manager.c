#include "dataset_manager.h"

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
int load_dataset( char ** dirs, int dir_num, Dataset * dataset, int max_per_folder ){

    int folder_counter = 0;
    int total_counter = 0;

    for( int i = 0; i < dir_num; i++ ){
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
    dataset->size = total_counter; 
    return total_counter;
}

int free_mri_image( mri_image * image ){
    // free( image->filename );
    free( image->inputs );
    free( image->pixels );

    return 0;
}

int free_dataset( Dataset * dataset ){

    for( u64 i = 0; i < dataset->size; i++ ){
         free_mri_image( &dataset->images[i] ); 
    }  

    free( dataset->images );
    return 0;
}

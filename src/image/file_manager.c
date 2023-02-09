#include "file_manager.h"

/*  File counter used in function main */
int count_file(char * foldername){
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
int load_dataset( char * dirs[], int dir_num, mri_image * dataset, int max_per_folder ){

    int max_check = 0;
    int n = 0;

    u8 *image_ptr = NULL;
    u8 *buffer_ptr = NULL;

    image_ptr = malloc( IMAGE_SIZE * sizeof(unsigned char));
    buffer_ptr = malloc( IMAGE_SIZE * sizeof(unsigned char));


    for( int i = 0; i < dir_num; i++ ){
        DIR *d;
        struct dirent *dir;
        d = opendir(dirs[i]);
        if (d) {
            while ((dir = readdir(d)) != NULL && !(max_check >= max_per_folder) ) {

                if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
                    continue;
                }
                char buf[512];
                snprintf(buf, sizeof(buf), "%s/%s", dirs[i], dir->d_name);

                dataset[n].inputs = (unsigned char *) prepare_image(buf, image_ptr, buffer_ptr);
                dataset[n].value = i;
                dataset[n].filename = "name";

                n++;
                max_check++; 
            }
            max_check = 0; 
            closedir(d);
        }
        else{
            printf("%s does not exist", dirs[i]);
            exit(0);
        }
    }
    free(image_ptr);
    free(buffer_ptr);


    return n;
}

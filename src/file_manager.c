#include "file_manager.h"
#include "image_processing.h"

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

int load_dataset( char * dirs[], int dir_num, mri_image * dataset, int max_per_folder ){
    int max_check = 0; //deleteme 
    int n = 0;
    for( int i = 0; i < dir_num; i++ ){
        DIR *d;
        struct dirent *dir;
        d = opendir(dirs[i]);
        if (d) {
            while ((dir = readdir(d)) != NULL && !(max_check >= max_per_folder) ) {

                // not clean
                if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
                    continue;
                }
                char buf[512];
                snprintf(buf, sizeof(buf), "%s/%s", dirs[i], dir->d_name);
                // printf("%s\n",buf); 
                dataset[n].inputs = (unsigned char *) prepare_image(buf);
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

    return n;
}

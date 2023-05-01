#include "directory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  File counter used in function main */
int count_file_in_directory( DIR * d ){
    int counter = 0;
    
    struct dirent *dir;
    if (d) {
        while ((dir = readdir(d)) != NULL ) {
            if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
                continue;
            }
            counter++;
        }
        rewinddir( d );
        // closedir(d);
    }
    else {
        return -1;
    }

    return counter;
 
}
 
int get_filemame_from_dir( char * folder_name, char *** files_name, int max ){

    DIR *d;
    struct dirent *dir;
    d = opendir(folder_name);

        
    if ( !d ) {
        printf(" Error : %s directory does not exist \n ", folder_name );
        return -1;
        // exit(0);
    }

    int directory_size = count_file_in_directory( d );

    if ( directory_size <= 0 )
        printf(" Error : %s directory is empty \n ", folder_name );

    directory_size = directory_size > max ? max : directory_size;

    (*files_name) = malloc( directory_size * sizeof(char*) );
    
    int count = 0;     
    while ((dir = readdir(d)) != NULL && count < max ) {
        
        if( strcmp( dir->d_name, ".") == 0 || strcmp( dir->d_name, "..") == 0 ) {
            continue;
        }

        (*files_name)[count] = malloc( STRING_SIZE * sizeof(char) );
        strcpy( (*files_name)[count], dir->d_name );
        count++;
        
    }
    closedir(d);

    return count;
}

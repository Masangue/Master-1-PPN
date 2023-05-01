 
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>



// #include "image_io.h"
// #include "context.h"


#include "type.h"
#include "global.h"

 
int count_file_in_directory( DIR * d );
int get_filemame_from_dir( char * folder_name, char *** files_name, int max );
// int count_file_in_directory(char * foldername);

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "filehandler.h" 

// exit(-1); 

/*
    error_handeling

*/


FILE_INFO get_file_info(char * path){ 
    FILE_INFO file_info;
    FILE * file = fopen(path, "r"); 

    if(file == NULL){ 
        printf("%s\n", path);
        printf("Can't read file\n"); 
        exit(1); 
    } 
    fseek(file, 0 , SEEK_END);  
    file_info.file_length = ftell(file);
    rewind(file); 
    file_info.file_content = (char*)malloc(file_info.file_length);  

    if(file_info.file_content == NULL){
        printf("File Content = NULL");  
        exit(1);
    }

    fread(file_info.file_content, 1, file_info.file_length, file); 
    fclose(file);

    return file_info; 
} 



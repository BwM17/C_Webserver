#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>  


struct file_info{
    char * file_content;  
    long file_length;    
    char * header;
};   

typedef struct file_info FILE_INFO; 
FILE_INFO get_file_info(char * path); 

#endif
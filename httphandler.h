#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H 

#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h>
#include <regex.h>  
#include <string.h>  
#include "filehandler.h" 

//#define RESPONSE_HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n" 
//#define ERROR_HEADER "HTTP/1.1 404\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n"

char * build_header(long file_length, char * path ,int error_code);
FILE_INFO parse_http_request(char* http_request);


#endif 

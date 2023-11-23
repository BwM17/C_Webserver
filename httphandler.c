#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h>
#include <regex.h>  
#include <string.h>  
#include "filehandler.h"
#include "httphandler.h"

char * build_header(long file_length, char * path, int error_code){    
    regex_t regex, regex1, regex2, regex3;  

    char * html_pattern = "\\html"; 
    char * css_pattern = "\\css"; 
    char * js_pattern = "\\js";   
    char * img_pattern = "\\img";
    char * template = "HTTP/1.1 %d \r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n";       
    char * file_type;  

    char * response = (char*)malloc(1000); 
    if(response == NULL){
        printf("memory can not be reserved"); 
    }  

    int html_ret = regcomp(&regex, html_pattern, 0);
    if(html_ret){  
        printf("REGEX_COMPILE error");  
        exit(-1);
    }    
    html_ret = regexec(&regex, path, 0, NULL, 0);  

    int css_ret = regcomp(&regex1, css_pattern, 0);   
    if(css_ret){  
        printf("REGEX_COMPILE error");  
        exit(-1);
    }     
    css_ret = regexec(&regex1, path, 0, NULL, 0); 

    int  js_ret = regcomp(&regex2, js_pattern, 0);
    if(js_ret){  
        printf("REGEX_COMPILE error");  
        exit(-1);
    } 
    js_ret = regexec(&regex2, path, 0, NULL, 0);   


    if(!html_ret){
        file_type = "text/html";
    }
    else if(!css_ret){
        file_type = "text/css";
    }
    else if(!js_ret){
        file_type = "application/js";
    }
    else{
        printf("Pattern not found");
    }
    

    int format = sprintf(response, template, error_code, file_type, file_length);   
    if(format < 0){  
        printf("Error_Format");
        exit(-1); 
    } 
    return response;   
}


FILE_INFO parse_http_request(char * http_request){     
    regex_t regex, regex1, regex2;  
    FILE_INFO file_info;   

    char method[50];  
    char path[50]; 
    char protocol[50];    
    char cwd[1000];
    
    char * html_pattern = "\\html"; 
    char * iframe_one_pattern = "\\/."; 
    char * iframe_two_pattern = "\\/..";  

    if(getcwd(cwd, sizeof(cwd)) == NULL){ 
        printf("cwd error\n");
        exit(-1);
    }  

    //create error Return 
    FILE_INFO error_ret = get_file_info("/home/redlotus/webserverV2/src/error.html");        
    error_ret.header = build_header(error_ret.file_length, "html", 404);  

    sscanf(http_request, "%s %s %s", method, path, protocol); 

    if(strcmp(method, "GET") != 0){   
        return error_ret;
    }     
    else if(strcmp(protocol, "HTTP/1.1") != 0){ 
        return error_ret;
    }

    int html_ret = regcomp(&regex, html_pattern, 0);
    if(html_ret){  
        printf("REGEX_COMPILE error");  
        exit(-1);
    }   
    html_ret = regexec(&regex, http_request, 0, NULL, 0); 
    int iframe_one_ret = regcomp(&regex1, iframe_one_pattern, 0);   
    if(iframe_one_ret){
        printf("REGEX_COMPILE error");  
        exit(-1);
    }  
    
    iframe_one_ret = regexec(&regex1, path, 0, NULL, 0); 
    int iframe_two_ret = regcomp(&regex2, iframe_two_pattern, 0);
    if(iframe_two_ret){ 
        printf("REGEX_COMPILE error");  
        exit(-1);
    } 
    iframe_two_ret = regexec(&regex2, path, 0, NULL, 0); 
    
    //test if https contains html 
    if(!html_ret){
        if(strcmp(path, "/") == 0){ 
            strcat(path, "src/index.html");  
            strcat(cwd, path);  
            file_info = get_file_info(cwd);  
            file_info.header = build_header(file_info.file_length, path, 200); 
            return file_info;
        }  
        else{  
            strcat(cwd, ""); 
            strcat(path, ".html"); 
            strcat(cwd, "/src");
            strcat(cwd, path);  
            file_info = get_file_info(cwd); 
            file_info.header = build_header(file_info.file_length, path, 200);
            return file_info; 
        }
    }  
    /*
    else if(!iframe_one_ret){ 
        return error_ret;
    } 
    else if(!iframe_two_ret){ 
        return error_ret;
    } 
    */
    else{   
        strcat(cwd, path); 
        file_info = get_file_info(cwd); 
        file_info.header = build_header(file_info.file_length, path, 200); 
        return file_info;
    }
}




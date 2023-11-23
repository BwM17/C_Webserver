#include <stdlib.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>  

#include "filehandler.h" 
#include "httphandler.h"

#define BACK_LOG 10


//threading ! 
//keep socket alive

int main(void){ 
    //create favicon check 

    regex_t regex, regex2;

    int favicon_ret = regcomp(&regex, "\\favicon.ico", 0); 
    if(favicon_ret){ 
        exit(-1);
    }  
    

    int webserver = socket(AF_INET, SOCK_STREAM, 0); 
    if(webserver < 0){
        perror("socket"); 
        exit(-1); 
    } 

    int optval = 1; 
    if(setsockopt(webserver, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))){
        perror("setsockopt"); 
        exit(-1); 
    }


    struct sockaddr_in webserver_addr;  
    webserver_addr.sin_family = AF_INET; 
    webserver_addr.sin_port =  htons(80);  
    webserver_addr.sin_addr.s_addr = htonl(INADDR_ANY); 


    int create = bind(webserver, (struct sockaddr*)&webserver_addr, sizeof(webserver_addr));
    if(create < 0){    
        perror("bind"); 
        exit(-1); 
    }

    while(1){
        printf("listening...\n");
        if (listen(webserver, BACK_LOG) == -1)
        {
            perror("listen");
            exit(-1);
        }

        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);

        int client_sock = accept(webserver, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0)
        {
            perror("accept");
            exit(-1);
        }

        char http_request[4096];
        int http_request_len = sizeof(http_request);

        if (recv(client_sock, &http_request, http_request_len, 0) < 0)
        {
            perror("recv");
            exit(-1);
        }  

        //printf("%s\n", http_request); 

        favicon_ret = regexec(&regex, http_request, 0, NULL, 0); 
        if(!favicon_ret){   
            continue;
        } 
        else{ 
            FILE_INFO response = parse_http_request(http_request);  

            if (send(client_sock, response.header, strlen(response.header), 0) < 0)
            {
                exit(-1);
            }

            // send file content
            if (send(client_sock, response.file_content, response.file_length, 0) < 0)
            {
                exit(-1);
            }  
            printf("sending data...\n");
        }
        close(client_sock); 
    }  

    close(webserver);
}

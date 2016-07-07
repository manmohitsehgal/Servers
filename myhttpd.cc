
const char * usage =
"                                                               \n"
"daytime-server:                                                \n"
"                                                               \n"
"Simple server program that shows how to use socket calls       \n"
"in the server side.                                            \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   daytime-server <port>                                       \n"
"                                                               \n"
"Where 1024 < port < 65536.             \n"
"                                                               \n"
"In another window type:                                       \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where daytime-server  \n"
"is running. <port> is the port number you used when you run   \n"
"daytime-server.                                               \n"
"                                                               \n"
"Then type your name and return. You will get a greeting and   \n"
"the time of the day.                                          \n"
"                                                               \n";


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "concurrency.h"
#include "directories.h"

int QueueLength = 5;

int main(int argc, char ** argv){
    // Print usage if not enough arguments
    if (argc < 2){
        fprintf(stderr, "%s", usage);
        exit(-1);
    }
    
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    
    // Get the port from the arguments
    int port;
    char* flag;
    if(argc == 2){
        port = atoi(argv[1]);
        flag = strdup("-n");
    }
    else{
        port = atoi(argv[2]);
        flag = argv[1];
    }
    
    // Set the IP address and port for this server
    struct sockaddr_in serverIPAddress;
    memset(&serverIPAddress, 0, sizeof(serverIPAddress));
    serverIPAddress.sin_family = AF_INET;
    serverIPAddress.sin_addr.s_addr = INADDR_ANY;
    serverIPAddress.sin_port = htons((u_short) port);
    
    // Allocate a socket
    int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
    if (masterSocket < 0){
        perror("socket");
        exit( -1 );
    }
    
    // Set socket options to reuse port. Otherwise we will have to wait about 2 minutes before reusing the same port number
    int optval = 1;
    int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(int));
    
    // Bind the socket to the IP address and port
    int error = bind(masterSocket,(struct sockaddr *)&serverIPAddress,sizeof(serverIPAddress));
    if (error){
        perror("bind");
        exit(-1);
    }
    
    // Put socket in listening mode and set the size of the queue of unprocessed connections
    error = listen(masterSocket, QueueLength);
    if (error) {
        perror("listen");
        exit( -1 );
    }
    
    if(strcmp(flag,"-n")==0)
        noConcurrency(masterSocket);
    else if(strcmp(flag,"-f")==0)
        processConcurrency(masterSocket);
    else if(strcmp(flag,"-t")==0)
        threadConcurrency(masterSocket);
    else if(strcmp(flag,"-p")==0)
        poolThreadConcurrency(masterSocket);
}

void processTimeRequest(int fd){
    int n;
    unsigned char newChar;
    
    char buf[10000];
    n = read(fd,buf,10000);
    
    //Get file name
    char temp[256];
    int count = 0;
    for(int i=0;i<256; i++) {
        if(buf[i]==' ') {
            i++;
            while(buf[i]!=' ') {
                temp[count] = buf[i];
                count++;
                i++;
            }
            temp[count] = '\0';
            break;
        }
    }
    
    char *filename = (char*) malloc(sizeof(char)*count+1);
    strncpy(filename, temp, count+1);
    
    char *fileNotFoundHeader = strdup("HTTP/1.0 404 File Not Found\015\012Server: CS 252 lab5\015\012Content-type: text/html\015\012\015\012File not found");
    
    //if filename is / open index.html
    if(strcmp(filename,"/") == 0)
        defaultAdd(fd,n,fileNotFoundHeader);
    else if(strstr(filename,"/cgi-bin")!=NULL)
        cgibin(fd,n,filename,fileNotFoundHeader);
    else if(strstr(filename,"/icons")!=NULL)
        icons(fd,n,filename,fileNotFoundHeader);
    else if(strstr(filename,"/dir1") != NULL){}
    else
        simpleComplex(fd,n,filename,fileNotFoundHeader);        // Simple Test and Complex Test
}

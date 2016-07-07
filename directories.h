void simpleComplex(int,int,char *,char *);
void defaultAdd(int,int,char *);
void cgibin(int,int,char *,char *);
void icons(int,int,char *,char *);

void icons(int fd,int n, char *filename, char *fileNotFoundHeader){
    if(strstr(filename,".html")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: text/html\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".gif")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".svg")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".jpg")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".jpeg")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: text/plain\015\012\015\012");
        write(fd,header,strlen(header));
    }
    
    int lenPrefix = strlen("http-root-dir");
    int lenPostfix = strlen(filename);
    
    char *fullPath = (char*) malloc(lenPrefix+lenPostfix-1);
    strcat(fullPath,"http-root-dir");
    strcat(fullPath,filename);
    
    int inFile = open(fullPath, O_RDONLY);
    if(inFile == -1) {
        write(fd,fileNotFoundHeader,strlen(fileNotFoundHeader));
    }
    char buffer[10240];
    
    while((n = read(inFile,buffer,10240))>0) {
        write(fd,buffer,n);
    }
    close(inFile);
}

void cgibin(int fd,int n, char *filename, char *fileNotFoundHeader){
    char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012");
    write(fd,header,strlen(header));
    int lenPrefix = strlen("/homes/msehgal/lab5-src/http-root-dir");
    int lenPostfix = strlen(filename);
    
    char *fullPath = (char*) malloc(lenPrefix+lenPostfix-1);
    
    strcat(fullPath,"/homes/msehgal/lab5-src/http-root-dir");
    strcat(fullPath,filename);
    
    int hasArguments = 0;
    char *afterQues;
    
    if(strstr(fullPath,"finger")==NULL)
        setenv(strdup("REQUEST_METHOD"),strdup("GET"),1);
    
    if((afterQues=strchr(fullPath,'?'))!=NULL) {
        hasArguments = 1;
        afterQues++;
        setenv(strdup("REQUEST_METHOD"),strdup("GET"),1);
        char *query_string = (char*)malloc(sizeof(1024));
        strcat(query_string,"QUERY_STRING=");
        strcat(query_string,afterQues);
        setenv("QUERY_STRING",query_string,1);
    }
    
    int tmpin = dup(0);
    int tmpout = dup(1);
    int fdin = open(fullPath, O_RDONLY);
    if(fdin == -1 && !hasArguments)
        write(fd,fileNotFoundHeader,strlen(fileNotFoundHeader));
    
    dup2(fdin,0);
    dup2(fd,1);
    close(fdin);
    
    int pid = fork();
    if(pid == 0){
        if(strstr(fullPath,"finger")!=NULL){
            if(!hasArguments)
                execl(fullPath,"finger",NULL);
            else
                execl("/homes/msehgal/lab5-src/http-root-dir/cgi-bin/finger","finger",afterQues,NULL);
        }
        else{
            if(!hasArguments)
                execl(fullPath,NULL);
            else
                execl(fullPath,afterQues,NULL);
        }
        _exit(1);
    }
    
    dup2(tmpin,0);
    dup2(tmpout,1);
    close(tmpin);
    close(tmpout);
    
}

void defaultAdd(int fd, int n, char *fileNotFoundHeader){
    char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: text/html\015\012\015\012");
    write(fd,header,strlen(header));
    
    int inFile = open("http-root-dir/htdocs/index.html", O_RDONLY);
    if(inFile == -1)
        write(fd,fileNotFoundHeader,strlen(fileNotFoundHeader));
    char buffer[1024];
    while((n = read(inFile,buffer,1024))>0) {
        write(fd,buffer,n);
    }
    close(inFile);
}


void simpleComplex(int fd, int n, char *filename, char *fileNotFoundHeader){
    if(strstr(filename,".html")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: text/html\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".gif")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".svg")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".jpg")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else if(strstr(filename,".jpeg")!=NULL) {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: image/gif\015\012\015\012");
        write(fd,header,strlen(header));
    }
    else {
        char* header = strdup("HTTP/1.0 200 Document follows\015\012Server: CS 252 lab5\015\012Content-type: text/plain\015\012\015\012");
        write(fd,header,strlen(header));
    }
    
    int lenPrefix = strlen("http-root-dir/htdocs");
    int lenPostfix = strlen(filename);
    
    char *fullPath = (char*) malloc(lenPrefix+lenPostfix-1);
    strcat(fullPath,"http-root-dir/htdocs");
    strcat(fullPath,filename);
    
    
    int inFile = open(fullPath, O_RDONLY);
    if(inFile == -1) {
        write(fd,fileNotFoundHeader,strlen(fileNotFoundHeader));
    }
    char buffer[10240];
    
    while((n = read(inFile,buffer,10240))>0) {
        write(fd,buffer,n);
    }
    close(inFile);
}

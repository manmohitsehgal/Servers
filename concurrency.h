void requestProcess(int);
void validConcurrency(int);
void worketPool(int);
void concurrentProcess(int);
void concurrentThread(int);
void poolConcurrency(int);
void processTimeRequest(int);


/* MUTEX'S */

pthread_mutex_t mutex;
pthread_mutexattr_t mutex2;



void workerPool(int socket) {
    while (1){
        // Accept incoming connections
        struct sockaddr_in clientIPAddress;
        int alen = sizeof(clientIPAddress);

        pthread_mutex_lock(&mutex); 
        int slaveSocket = accept(socket,
                (struct sockaddr *)&clientIPAddress,
                (socklen_t*)&alen);

        pthread_mutex_unlock(&mutex);

        // check if slave is less then 0, if it means that means its an error
        // if its not just process the given/incoming request

        if(slaveSocket < 0){
            perror( " accept error");
            exit(-1);
        }
        requestProcess(slaveSocket);
        close(slaveSocket);
    }
}

 
void concurrentProcess(int masterSocket){

		while ( 1 ) {

		    // Accept incoming connections
		    struct sockaddr_in clientIPAddress;
		    int alen = sizeof( clientIPAddress );
		    int slaveSocket = accept( masterSocket, (struct sockaddr *)&clientIPAddress, (socklen_t*)&alen);

		    if ( slaveSocket < 0 ) {
		      perror( "accept" );
		      exit( -1 );
		    }

			pid_t pid = fork();
			if( pid == 0 ) {
		    		// Process request.
		   	 	requestProcess( slaveSocket );
		   	 	close( slaveSocket );
				exit(EXIT_SUCCESS);
		   	}
		    // Close socket
		    close( slaveSocket );
		  }
}


void concurrentThread(int masterSocket){
	while ( 1 ) {

		// Accept incoming connections

		pthread_t tid;
		pthread_attr_t attr;
				    
		pthread_attr_init(&attr);
		pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket, (struct sockaddr *)&clientIPAddress, (socklen_t*)&alen);

		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}

		pthread_create(&tid, &attr, (void * (*)(void *)) requestProcess,(void *) slaveSocket);
		pthread_detach(tid);

	}
}

void poolConcurrency(int masterSocket){

			pthread_mutexattr_init(&mutex2);
			pthread_mutex_init(&mutex, &mutex2);
		
			pthread_t tid[5];
			pthread_attr_t attr;
		
			pthread_attr_init(&attr);
			pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);            
		
			for(int i = 0; i < 5; i++){
		    		pthread_create(&tid[i], &attr, (void * (*)(void *))workerPool, (void *)masterSocket);
			}
		
			pthread_join(tid[0], NULL);
}



void validConcurrency(int masterSocket){
	  while ( 1 ) {

	    // Accept incoming connections
	    struct sockaddr_in clientIPAddress;
	    int alen = sizeof( clientIPAddress );
	    int slaveSocket = accept( masterSocket, (struct sockaddr *)&clientIPAddress, (socklen_t*)&alen);

	    if ( slaveSocket < 0 ) {
	      perror( "accept" );
	      exit( -1 );
	    }

	    // Process request.
	    requestProcess( slaveSocket );
	    close( slaveSocket );
	  }
}

void requestProcess(int socket)
{
    processTimeRequest(socket);
    close(socket);
}



#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include <netdb.h>
#include "packet-format.h"
#include <sys/stat.h>


#define FILENAME argv[1]
#define HOSTNAME argv[2]
#define PORTARG  argv[3]
#define WINDOW_SIZE argv[4]
int getFileSize(const char *filename){
	struct stat st;
	stat(filename, &st);
	return st.st_size;

}

int main(int argc, char const *argv[]){

	FILE *fp;
	int sockfd;
	struct hostent *host = gethostbyname(HOSTNAME);

	if(!host){
		perror("gethostbyname");
	}

	long port = strtol(PORTARG, NULL, 10);
	int window_size = strtol(WINDOW_SIZE, NULL, 10);
	struct sockaddr_in servaddr, cliaddr;

	memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 


    HOSTNAME = inet_ntoa(*((struct in_addr*) 
                           host->h_addr_list[0]));
	


	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(HOSTNAME); 
    servaddr.sin_port = htons(port); 

	if(argc != 5){
		fprintf(stderr, "%s\n", "INVALID ARGUMENTS");
		return -1;
	}

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("SOCKET FAIL");
		return -1;
	}

	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		
		perror("BIND FAIL");
		return -1;
	}

	fp = fopen(FILENAME, "r");


	int seqNum = 0;
	int fileSize = getFileSize(FILENAME);
	int numPackets = fileSize / CHUNK_SIZE;
	int lastPacket = fileSize % CHUNK_SIZE;
	
	if(seqNum && window_size && host && fp && numPackets && lastPacket){
		;
	}



	return 0;

}
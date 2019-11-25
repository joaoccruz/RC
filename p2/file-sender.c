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

#define max(a,b) a > b ? a:b 

int getFileSize(const char *filename){
	struct stat st;
	stat(filename, &st);
	return st.st_size;

}

void generatePackets(int totalNumPackets, data_pkt_t *myPackets, FILE *fp){
	for(int i = 0; i < totalNumPackets;){
		int n, lastPacket;
		if(++i < totalNumPackets){
			n = 1000;
		}else{
			n = lastPacket; 
		}

		char buffer[1000];
		fgets(myPackets[i-1].data, 1000, fp);
		
		myPackets[i-1].seq_num = i;
		
		
	}

}

void sendChunks(data_pkt_t *myPackets,int begin,int windowSize,int s,int *currentAcks){
	for(int i = 0; i < windowSize; i++){
		send(s, (void *) &myPackets[begin+i], sizeof(data_pkt_t), 0);
		//fprintf(stderr, "sent %s\n", myPackets[begin+i].data);
	}
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
	struct timeval timeout;
	timeout.tv_sec = 5*TIMEOUT;
	timeout.tv_usec = 0;
	
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&timeout, sizeof(struct timeval));
	
	
	int sockLen = sizeof(servaddr);
	if(connect(sockfd, (struct sockaddr *)&servaddr, sockLen) < 0){
		perror("CONNECT FAIL");
		return -1;
	}

	fp = fopen(FILENAME, "r");


	int seqNum = 1;
	int fileSize = getFileSize(FILENAME);
	int numPackets = fileSize / CHUNK_SIZE;
	int lastPacket = fileSize % CHUNK_SIZE;

	int totalNumPackets = numPackets+(lastPacket>0);
	data_pkt_t myPackets[totalNumPackets];

	generatePackets(totalNumPackets, myPackets, fp);

	for(int i = 0; i < totalNumPackets; i++){
		printf("%s", myPackets[i].data);	
	}


	char *acks = malloc(totalNumPackets * sizeof(char));
	memset(acks, 0, totalNumPackets*sizeof(char));

	sleep(1);
	sendChunks(myPackets, 0, window_size, sockfd, 0);
	char buffer[1000];

	//int r = recv(sockfd, buffer, 1000, 0);
	int r;

	if(r){
		fprintf(stderr, "%s\n", buffer);

	}else{
		fprintf(stderr, "something went wrong\n");
	}
	fprintf(stderr,"%s\n", "exiting");
	return 0;

}
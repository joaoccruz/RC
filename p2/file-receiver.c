#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "packet-format.h"

FILE *fp;
void handleData(char *data){
	fprintf(fp, "%s", data);
}

int main(int argc, char const *argv[]){
	fp = fopen(argv[1], "w");
	int sockfd, window_size;
	long port;
	struct sockaddr_in myAddr, senderAddr;

	port = strtol(argv[2], NULL, 10);
	window_size = strtol(argv[3], NULL, 10);

	senderAddr.sin_family = AF_INET;
	myAddr.sin_family = AF_INET; 
	myAddr.sin_addr.s_addr = INADDR_ANY; 
    myAddr.sin_port = htons(port); 

	if(argc != 4){
		return -1;
	}


	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("SOCKET FAIL");
		return -1;
	}

	if(bind(sockfd, (struct sockaddr*)&myAddr, sizeof(myAddr)) < 0){	
		perror("BIND FAIL");
		return -1;
	}
	


	void *buffer = malloc(sizeof(data_pkt_t));
	int exit = 1000;
	char test[1000];
	strcpy(test, "works");

	


	while(exit){
		//recvfrom(sockfd, buffer, sizeof(data_pkt_t), 0, (struct sockaddr *)&senderAddr, (socklen_t *)&len);
		free(buffer);
		buffer = malloc(sizeof(data_pkt_t));
		int sockLen = sizeof(myAddr);
		recvfrom(sockfd, buffer, sizeof(data_pkt_t), 0, (struct sockaddr*) &senderAddr, &sockLen);
		

		data_pkt_t *casted = (data_pkt_t *)buffer;
		//fprintf(stderr, "%s\n",casted->data );
		handleData(casted->data);
		if(strlen(casted->data) != 1000)
			break;

		//send(sockfd, test, 1000, 0);
		
	}

	sendto(sockfd, test, 1000, 0, (struct sockaddr *)&senderAddr, sizeof(senderAddr));
	return 0;

}
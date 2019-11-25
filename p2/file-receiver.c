#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "packet-format.h"

int main(int argc, char const *argv[]){

	FILE *fp = fopen(argv[1], "w");
	int sockfd, window_size;
	long port;
	struct sockaddr_in servaddr, cliaddr;

	port = strtol(argv[2], NULL, 10);
	window_size = strtol(argv[3], NULL, 10);

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(port); 

	if(argc != 4){
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
	


	void *buffer;
	char exit = 0;
	char test[1000];
	strcpy(test, "works");
	while(!exit){
		int sockLen = sizeof(servaddr);
		recv(sockfd, buffer, sizeof(data_pkt_t), 0);
		data_pkt_t *casted = (data_pkt_t *)buffer;
		//fprintf(stderr, "%s\n",casted->data );
		fprintf(fp, "%s", casted->data);

		//send(sockfd, test, 1000, 0);
	}

	return 0;

}
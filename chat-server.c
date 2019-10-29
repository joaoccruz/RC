#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char const *argv[]){

	int server_port, server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *message;

	if(argv[1] != NULL){
		server_port = argv[1];
	}
	else{
		exit(-1);
	}

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		exit(-1);
	}

	address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(server_port);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
		exit(-1);
	}
	if(listen(server_fd, 3) < 0){
		perror("listen");
		exit(-1);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
		perror("accept");
		exit(-1);
	}
	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");

	return 0;

}

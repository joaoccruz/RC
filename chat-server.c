//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include "common.h"


int main(int argc , char *argv[]){
	if(argc != 2){
		return -1;
	}

	int socketFD = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if(socketFD < 0){
		fprintf(stderr, "DEBUG: SOCKET CREATE FAIL %d\n", socketFD);
		return -1;
	}

	long PORT = strtol(argv[1], NULL, 10);
	address serverAddress;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	serverAddress.sin_port = htons(PORT);


	int status = bind(socketFD, (struct sockaddr *)&serverAddress,  sizeof(serverAddress));
	if(status < 0){
		fprintf(stderr, "DEBUG: BIND FAIL\n");
		return -1;
	}

	status = listen(socketFD, 1000);
	
	if(status < 0){
		fprintf(stderr, "DEBUG: LISTEN FAIL\n");
		return -1;
	}

	fd_set fdList;
	fd_set writeFdl;
	int clientList[1000] = {0};
	int clientNum = 0;
	int fdlSize = socketFD+1;
	FD_ZERO(&fdList);
	FD_ZERO(&writeFdl);

	FD_SET(socketFD, &fdList);
	
	for(;;){
		
		if (select(fdlSize, &fdList, &writeFdl, NULL, NULL) < 0){
			return -1;
		}

		for(int i = 0; i < fdlSize; i++){
			if(FD_ISSET(i, &fdList)){

				struct sockaddr *clientAddr;
				int a;
				int newClient = accept(socketFD, clientAddr, &a);
				clientList[clientNum] = newClient;
				
				// INFORM
				for(int j = 0; j < clientNum; j++){
					if(clientList[j]){
						write(clientList[j], "hi\n", 3);
						printf("%d\n", FD_ISSET(clientList[j], &fdList));
						int error_code = 0;
						int error_code_size = sizeof(error_code);
						getsockopt(clientList[j], SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
						if(error_code){
							clientList[j] = 0;
						}
					}
				}
				/*

				address* realAdd = (address*)&clientAddr;
				struct in_addr ipAddr = realAdd->sin_addr;
				

				char clientAddrStr[BUFF_SIZE];
				inet_ntop(AF_INET, &ipAddr, clientAddrStr, INET_ADDRSTRLEN);

				int netPort = ntohs(realAdd->sin_port);
*/
				clientNum++;
				FD_SET(newClient, &fdList);
				FD_SET(newClient, &writeFdl);
			}

		}


		/*while(newClient){
			printf("%d\n", newClient);
			if(newClient < 0){
				fprintf(stderr, "DEBUG: ACCEPT FAIL\n");
				return -1;
			}

			char buffer[BUFF_SIZE];
			
			for(int i = 0; i < clientNum; i++){
				if(clientList[i]){
					fprintf(stderr, "SENDING HI TO %d\n", clientList[i]);
					write(clientList[i], "hi\n", 2);
					int error_code = 0;
					int error_code_size = sizeof(error_code);
					getsockopt(clientList[i], SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
					if(error_code){
						clientList[i] = 0;
					}

				}

			}

			clientList[clientNum++] = newClient;
			newClient = accept(socketFD, clientAddr, &a);
			puts("AA");
		}*/

	}
	close(socketFD);
	return 0;
}
/*
	//set of socket descriptors

	//a message
	char *message;

	//initialise all client_socket[] to 0 so not checked
	memset(0, client_socket, 1000);

	if(argv[1] != NULL){
		server_port = atoi(argv[1]);
	}

	if(argv[1] == NULL){
		return -1;
	}

	//create a master socket
	if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){
		//perror("socket failed");
		return -1;
	}

	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0){
		//perror("setsockopt");
		return -1;
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(server_port);

	//bind the socket to localhost port 8888
	if(bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0){
		//perror("bind failed");
		return -1;
	}

	//try to specify maximum of 3 pending connections for the master socket
	if(listen(master_socket, 1000) < 0)	{
		//perror("listen");
		return -1;
	}

	//accept the incoming connection
	addrlen = sizeof(address);

	while(TRUE){
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set
		for(i = 0; i < max_clients; i++){
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if(sd > 0){
				FD_SET(sd, &readfds);
			}

			//highest file descriptor number, need it for the select function
			if(sd > max_sd){
				max_sd = sd;
			}
		}

		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if((activity < 0) && (errno!=EINTR)){
			printf("select error");
			//return -1;?
		}

		//If something happened on the master socket ,
		//then its an incoming connection
		if(FD_ISSET(master_socket, &readfds)){
			if((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
				//perror("accept");
				return -1;
			}

			//inform user of socket number - used in send and receive commands
			
			message = ("%s:%d joined.\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));


			//send new connection greeting message
			if(send(new_socket, message, strlen(message), 0) != strlen(message)){
				perror("send");
			}

			//puts("Welcome message sent successfully");

			//add new socket to array of sockets
			for(i = 0; i < max_clients; i++){
				//if position is empty
				if(client_socket[i] == 0){
					client_socket[i] = new_socket;
					//printf("Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}

		//else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++){
			sd = client_socket[i];
			if (FD_ISSET(sd, &readfds)){
				//Check if it was for closing , and also read the
				//incoming message
				if((valread = read( sd , buffer, 1024)) == 0){
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}

				//Echo back the message that came in
				else{
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					send(sd , buffer , strlen(buffer) , 0 );
				}
			}
		}
	}
*/

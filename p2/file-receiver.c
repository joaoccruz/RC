#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

typedef struct __attribute__(( __packed__)) ack_pkt_t {
	uint32_t seq_num;
	uint32_t selective_acks;
} ack_pkt_t;

int main(int argc, char const *argv[]){

	if(argc != 3){
		return -1;
	}

	return 0;

}
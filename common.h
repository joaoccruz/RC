#define BUFF_SIZE 4097
#define address struct sockaddr_in  
#include <sys/socket.h> 


int isValidMessage(char*);
int handleFd(int, fd_set*);
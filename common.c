#include "common.h"
#include <string.h>

int isValidMessage(char *buff){
	for(int i = 0; i < strlen(buff); i++){
		if(buff[i] == '\n')
			return 1;
	}
	return 0;
}

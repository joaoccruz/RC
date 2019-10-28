int main(int argc, char const *argv[]){

	int server_port;

	if(argv[1] != NULL){
		server_port = argv[1];
	}
	else{
		exit(-1);
	}

	return 0;

}

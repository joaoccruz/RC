all: build

build: server client common
	gcc common.o chat-server.o -o chat-server
	gcc common.o chat-client.o -o chat-client

test:
	zip project1 chat-server.c chat-client.c Makefile
	./test-submission.sh project1.zip

server:
	gcc -c chat-server.c -o chat-server.o
client:
	gcc -c chat-client.c -o chat-client.o
common:
	gcc -c common.c -o common.o


clean:
	rm *.o
	rm chat-server
	rm chat-client
	rm project1.zip

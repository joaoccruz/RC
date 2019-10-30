all: build

build: server client

test:
	zip project1 chat-server.c chat-client.c Makefile
	./test-submission.sh project1.zip

server:
	gcc chat-server.c -o chat-server
client:
	gcc chat-client.c -o chat-client

clean:
	rm chat-client
	rm chat-server
	rm project1.zip

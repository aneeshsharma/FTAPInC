CC = gcc

all: server client

client: client.c fileTransfer.c
	$(CC) client.c -o client

server: server.c fileTransfer.c
	$(CC) server.c -o server

clean:
	rm *.o server client
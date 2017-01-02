CC = gcc
CFLAGS = -Wall -std=c99 -Werror

PORT=12345
CFLAGS += -DPORT=\$(PORT)

all: mismatcher

mismatcher: questions.c qtree.c mismatch_server.c
	$(CC) $(CFLAGS) -g questions.c qtree.c mismatch_server.c -o mismatch_server
	
clean:  
	rm mismatch_server

CC = gcc
CFLAGS = -Wall -std=c99

all: clean questions qtree categorizer 

questions:
	$(CC) $(CFLAGS) questions.c test1.c -o test1
	
qtree:					    	
	$(CC) $(CFLAGS) questions.c qtree.c test2.c -o test2

categorizer:
	$(CC) $(CFLAGS) categorizer.c qtree.c questions.c -o categorizer

clean:  
	rm -f test1 test2 categorizer

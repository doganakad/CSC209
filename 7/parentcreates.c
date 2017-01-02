#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {

	int i;
	int n;
	int num_kids;

	if(argc != 2) {
		fprintf(stderr, "Usage: forkloop <numkids>\n");
		exit(1);
	}

	num_kids = atoi(argv[1]);

	for(i = 0; i < num_kids; i++) {
		n = fork();
		if(n == 0) {
			printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
			exit(1);
		}
		else
			printf("pid = %d, ppid = %d, i = %d\n", getpid(), getppid(), i);
	}
	for(i = 0; i < num_kids; i++) {
		if(wait(NULL) == -1) {
			perror("wait");
		}
	}

	return 0;
}

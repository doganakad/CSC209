#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

int seconds;
int i;

void handler(int code) {
	fprintf(stdout, "\nProgram ran %d seconds\n", seconds);
	fprintf(stdout, "Total number of reads: %d\n", i);
	exit(0);
}

int catch_signal(int sig, void (*handler)(int)) {
	struct sigaction newact;
	newact.sa_handler = handler;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	return sigaction(sig, &newact,NULL);
}
int main(int argc, char *argv[]){
	seconds = atoi(argv[1]);
	FILE *ptr;
	struct itimerval new, old;
	catch_signal(SIGALRM,handler);
	new.it_interval.tv_usec = 0;
	new.it_interval.tv_sec = 0;
	new.it_value.tv_sec = 0;
	new.it_value.tv_sec = (long int) seconds;
	setitimer(ITIMER_REAL, &new, &old);
	int error;
	int val;
	i = 0;
	ptr = fopen(argv[2],"rb");
	if (!ptr){
		fprintf(stderr, "Unable to open the file\n");
		return 1;
	}
	while(1){
		int b = random() % 100;
		fseek(ptr, b * sizeof(int), SEEK_SET);
		error = fread(&val, sizeof(int), 1, ptr);
		if (error == 1){
			fprintf(stdout, "%d\n", val);
			i++;
		}
		else{
			fprintf(stderr, "Error: value could not be read\n");
		}
	}
	error = fclose(ptr);
	if (error != 0){
		fprintf(stderr, "fclose failed\n");
		return 1;
	}
	return 0;
}
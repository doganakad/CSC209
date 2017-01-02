#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	FILE *ptr;
	int i;
	int error;
	int b;
	ptr = fopen(argv[1], "wb");
	if (!ptr){
		fprintf(stderr, "Unable to open the file\n");
		return 1;
	}
	for (i = 0; i < 100; i ++){
		b = random() % 100;
		fwrite(&b, sizeof(int),1,ptr);
	}
	error = fclose(ptr);
	if (error != 0){
		fprintf(stderr, "fclose failed\n");
		return 1;
	}
	return 0;
}
#include <stdio.h>

int main(){
	FILE * fp;
	FILE * fp1;
	FILE * fp2;
	FILE * fp3;
	int buffer[1];
	int total = 0;
	int i;
	fp = fopen("outputFile","a");
	i = 133;
	fprintf(fp,"%d\n",i);
	fclose(fp);
	fp1 = fopen("outputFile", "ab");
	fwrite(&i, sizeof(int), 1, fp1);
	fclose(fp1);
	fp2 = fopen("nameFile","w");
	fprintf(fp2,"Dogan Akad\n");
	fclose(fp2);
	fp3 = fopen("nameFile","rb");
	while (fread(&buffer, sizeof(int), 1, fp3) == 1) {
		for(i = 0; i < 1; i++){
			total += buffer[i];		
		}
	}
	fprintf(stdout, "%d\n", total);
	fclose(fp3);
	return 0;
}

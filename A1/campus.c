#include <stdio.h>
int main() {
	double latitude;
	double longitude;
	char label[80];

	int started = 0;

	while (fscanf(stdin, "%lf,%lf,%79[^\n])", &latitude, &longitude, label) == 3) {

		if (latitude < 43.65 || latitude > 43.67){
			fprintf(stderr, "out of bounds\n");
			continue;
		}


		if (longitude < -79.41 || longitude > -79.38){
			fprintf(stderr, "out of bounds\n");
			continue;
		}
		if (started)
			fprintf(stdout, "\n");
		else
			started = 1;
		fprintf(stdout, "%lf,%lf,%s", latitude, longitude, label);
	}
	fprintf(stdout, "\n");
	return 0;


}

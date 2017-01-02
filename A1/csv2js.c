#include <stdio.h>
int main() {
	double latitude;
	double longitude;
	char label[80];

	int started = 0;
	fprintf(stdout, "data=[\n");

	while (fscanf(stdin, "%lf,%lf,%79[^\n])", &latitude, &longitude, label) == 3) {

		if (latitude < -90 || latitude > 90){
			fprintf(stderr, "invalid latitude\n");
			continue;
		}


		if (longitude < -180 || longitude > 180){
			fprintf(stderr, "invalid longitude\n");
			continue;
		}
		if (started)
			fprintf(stdout, ",\n");
		else
			started = 1;
		fprintf(stdout, "{latitude: %lf, longitude: %lf, label: '%s'}", latitude, longitude, label);
	}
	fprintf(stdout, "\n];");
	return 0;


}
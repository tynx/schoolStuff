#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "ip_connection.h"
#include "bricklet_distance_ir.h"
#include "bricklet_distance_us.h"
#include "bricklet_temperature.h"

#define HOST "localhost"
#define PORT 4223
#define bricklet_ir_UID "qsX"
#define bricklet_us_UID "q9m"
#define bricklet_temp_UID "qw7"
#define RUN 20


int main() {

	FILE *pFile;
	pFile = fopen("results.csv", "a");

	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	DistanceIR dist_ir;
	distance_ir_create(&dist_ir, bricklet_ir_UID, &ipcon); 

	DistanceUS dist_us;
	distance_us_create(&dist_us, bricklet_us_UID, &ipcon); 

	Temperature t;
	temperature_create(&t, bricklet_temp_UID, &ipcon); 

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	int ir_dists[RUN] = {-1};
	int us_dists[RUN] = {-1};
	int temps[RUN] = {-1};
	uint16_t distance_ir;
	uint16_t distance_us;
	int16_t temperature;
	int i=0;
	for(i=0; i<RUN; i++){
		if(distance_ir_get_distance(&dist_ir, &distance_ir) < 0) {
			fprintf(stderr, "Could not get value, probably timeout\n");
			exit(1);
		}
		if(distance_us_get_distance_value(&dist_us, &distance_us) < 0) {
			fprintf(stderr, "Could not get value, probably timeout\n");
			exit(1);
		}
		if(temperature_get_temperature(&t, &temperature) < 0) {
			fprintf(stderr, "Could not get value, probably timeout\n");
			exit(1);
		}
		ir_dists[i] = distance_ir;
		us_dists[i] = distance_us;
		temps[i] = temperature;
		printf("DistanceIR: %f cm\n", distance_ir/10.0);
		printf("DistanceUS: %d\n", distance_us);
		printf("Temp: %d\n", temperature);

		nanosleep((struct timespec[]){{0, 500000000L}}, NULL);
	}

	int avg_distance_ir = 0;
	int avg_distance_us = 0;
	int avg_temp = 0;
	for(i=0; i<RUN; i++){
		avg_distance_ir += ir_dists[i];
		avg_distance_us += us_dists[i];
		avg_temp += temps[i];
	}

	avg_distance_ir /= RUN;
	avg_distance_us /= RUN;
	avg_temp /= RUN;

	printf("%d;%d;%d\n", avg_distance_ir, avg_distance_us, avg_temp);
	fprintf(pFile, "%d;%d;%d\n", avg_distance_ir, avg_distance_us, avg_temp);
	ipcon_destroy(&ipcon);
}

#include <stdio.h>
#include "ip_connection.h"
#include "bricklet_distance_us.h"
#include "bricklet_distance_ir.h"

#define HOST "localhost"
#define PORT 4223
#define USUID "q9m"
#define DEST "measurements/values"

// Callback function for distance value
void cb_distance_us(uint16_t distance, void *user_data) {
	(void)user_data; // avoid unused parameter warning
	fprintf(user_data, "%d\n", distance);
	fflush(user_data);
	printf("%d\n", distance);
}

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	DistanceUS distance_us;
	distance_us_create(&distance_us, USUID, &ipcon); 

	FILE *pFile;
	pFile = fopen(DEST, "a");

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}

	//Setting up us bricklet
	distance_us_set_moving_average(&distance_us, 100);
	distance_us_set_distance_callback_period(&distance_us, 500);

	// Register callback
	distance_us_register_callback(&distance_us,
					DISTANCE_US_CALLBACK_DISTANCE,
					(void *)cb_distance_us,
					pFile);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	fclose(pFile);
}

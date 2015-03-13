#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_distance_us.h"

#define HOST "localhost"
#define PORT 4223
#define UID "q9m" // Change to your UID

int main() {
    // Create IP connection
    IPConnection ipcon;
    ipcon_create(&ipcon);

    // Create device object
    DistanceUS dist;
    distance_us_create(&dist, UID, &ipcon); 

    // Connect to brickd
    if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
        fprintf(stderr, "Could not connect\n");
        exit(1);
    }
    // Don't use device before ipcon is connected

    // Get current distance value
    uint16_t distance;
    if(distance_us_get_distance_value(&dist, &distance) < 0) {
        fprintf(stderr, "Could not get value, probably timeout\n");
        exit(1);
    }

    printf("Distance Value: %d\n", distance);

    printf("Press key to exit\n");
    getchar();
    ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}

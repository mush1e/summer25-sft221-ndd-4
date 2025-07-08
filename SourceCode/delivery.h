#ifndef DELIVERY_H
#define DELIVERY_H

#include "mapping.h"

// Simple constants for the delivery system
#define MAX_SHIPMENTS 50
#define MAX_WEIGHT 5000      // kilograms
#define MAX_VOLUME 200       // cubic meters
#define NUM_TRUCKS 3

/**
 * Represents a package/shipment that needs to be delivered
 * Author: Mustafa Siddiqui
 */
struct Shipment {
    double weight;            // Weight in kilograms
    double volume;            // Volume in cubic meters (0.5, 2, or 5)
    struct Point destination; // Where to deliver (row, col)
};

/**
 * Represents one delivery truck with its cargo
 * Author: Mustafa Siddiqui
 */
struct Truck {
    struct Route route;                     // The path this truck follows
    struct Shipment cargo[MAX_SHIPMENTS];   // Packages in the truck
    int numShipments;                       // How many packages are loaded
    double currentWeight;                   // Total weight currently loaded
    double currentVolume;                   // Total volume currently loaded
    int truckNumber;                        // 0=Blue, 1=Green, 2=Yellow
};

/**
 * Structure to hold user input for validation
 * Author: Mustafa Siddiqui
 */
struct ShipmentInput {
    double weight;          // Weight entered by user
    double boxSize;         // Box size entered by user
    char destination[10];   // Destination string like "12L"
    int isValid;            // 1 if input is valid, 0 if not
};

/**
 * Simple result structure for when we try to assign a shipment
 * Author: Mustafa Siddiqui
 */
struct DeliveryResult {
    int success;            // 1 if we found a truck, 0 if no truck available
    int truckIndex;         // Which truck (0, 1, or 2)
    int needsDiversion;     // 1 if truck needs to go off route
    double distanceToGo;    // How far off route the truck needs to go
};

#endif
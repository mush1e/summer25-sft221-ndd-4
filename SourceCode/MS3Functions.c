/*
* Author: Mustafa Siddiqui
* Purpose: Implementation of MS3 delivery system functions
*/

#include "MS3FunctionSpecs.h"
#include <stdio.h>

/*
* Name: remainingCapacityKg
* Author: Mustafa Siddiqui
* Description: Returns the remaining weight capacity (kg) for the given truck
* FULLY IMPLEMENTED
*/
int remainingCapacityKg(const struct Truck *t) {
    // Check for NULL pointer
    if (t == NULL) {
        return 0;
    }
    
    // Check if truck is already at or over capacity
    if (t->currentWeight >= MAX_WEIGHT) {
        return 0;
    }
    
    // Calculate and return remaining capacity
    return MAX_WEIGHT - t->currentWeight;
}

/*
* Name: remainingVolumeM3
* Author: Mustafa Siddiqui
* Description: Returns the remaining volume capacity (in cubic metres) for the selected truck
* FULLY IMPLEMENTED
*/
double remainingVolumeM3(const struct Truck *t) {
    // Check for NULL pointer
    if (t == NULL) {
        return 0.0;
    }
    
    // Check if truck is already at or over volume capacity
    if (t->currentVolume >= MAX_VOLUME) {
        return 0.0;
    }
    
    // Calculate and return remaining volume
    return MAX_VOLUME - t->currentVolume;
}

/*
* Name: canFitShipment
* Author: Mustafa Siddiqui
* Description: Checks whether a shipment will be able to fit into specified truck
* FULLY IMPLEMENTED
*/
int canFitShipment(const struct Truck *t, const struct Shipment *s) {
    // Check for NULL pointers
    if (t == NULL || s == NULL) {
        return 0;  // Cannot fit if either pointer is NULL
    }
    
    // Check if shipment weight exceeds remaining weight capacity
    int remainingWeight = remainingCapacityKg(t);
    if (s->weight > remainingWeight) {
        return 0;  // Too heavy
    }
    
    // Check if shipment volume exceeds remaining volume capacity
    double remainingVolume = remainingVolumeM3(t);
    if (s->volume > remainingVolume) {
        return 0;  // Too large
    }
    
    // Shipment can fit in both weight and volume constraints
    return 1;
}

/*
* Name: assignShipment
* Author: Mustafa Siddiqui
* Description: Places the shipment in the truck array using the project's set rules
* STUB IMPLEMENTATION
*/
int assignShipment(struct Truck trucks[], int numTrucks, const struct Shipment *s) {
    // TODO: Implement this function
    // Stub returns -1 (no truck found) for now
    return -1;
}
/*
* Author: Mustafa Siddiqui
* Purpose: Implementation of MS3 delivery system functions
*/

#include "MS3FunctionSpecs.h"
#include <stdio.h>
#include <float.h>
#include "MS3FunctionSpecs.h"
#include "delivery.h"
#include "mapping.h"

/*
* Name: remainingCapacityKg
* Author: Mustafa Siddiqui
* Description: Returns the remaining weight capacity (kg) for the given truck
* FULLY IMPLEMENTED
*/
int remainingCapacityKg(const struct Truck* t) {
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
double remainingVolumeM3(const struct Truck* t) {
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
int canFitShipment(const struct Truck* t, const struct Shipment* s) {
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
* Name: findClosestTruck
* Author: Mustafa Siddiqui
* Description: Finds the truck that comes closest to the destination point
* FULLY IMPLEMENTED
*/
int findClosestTruck(const struct Truck trucks[], int numTrucks, const struct Point destination, const struct Map* map) {
    if (trucks == NULL || numTrucks <= 0 || map == NULL) {
        return -1;
    }

    int closestTruckIndex = -1;
    double shortestDistance = DBL_MAX;

    for (int i = 0; i < numTrucks; i++) {
        double truckDistance = calculateRouteDistance(&trucks[i], destination, map);

        // If this truck can reach the destination and is closer than previous best
        if (truckDistance >= 0.0 && truckDistance < shortestDistance) {
            shortestDistance = truckDistance;
            closestTruckIndex = i;
        }
    }

    return closestTruckIndex;
}

/*
* Name: calculateRouteDistance
* Author: Judd niemi
* Description: Calculates the shortest distance from a truck's route to a destination point
* FULLY IMPLEMENTED
*/
static int debugCounter = 0;

double calculateRouteDistance(const struct Truck* truck,
    const struct Point destination,
    const struct Map* map) {

    if (!truck || !map) return -1.0;

    for (int i = 0; i < truck->route.numPoints; i++) {
        if (truck->route.points[i].row == destination.row &&
            truck->route.points[i].col == destination.col) {
            return 0.0;
        }
    }

    double shortestDistance = DBL_MAX;

    for (int i = 0; i < truck->route.numPoints; i++) {
        struct Point routePoint = truck->route.points[i];

        // Removed debug print here

        double euclid = distance(&routePoint, &destination);
        if (euclid < shortestDistance) {
            shortestDistance = euclid;
        }
    }

    if (shortestDistance > 10.0) {
        return -1.0;
    }

    return shortestDistance;
}




/*
* Name: addShipmentToTruck
* Author: Mustafa Siddiqui
* Description: Adds a shipment to the specified truck's cargo
* FULLY IMPLEMENTED
*/
int addShipmentToTruck(struct Truck* truck, const struct Shipment* shipment) {
    if (truck == NULL || shipment == NULL) {
        return 0;
    }

    // Check if truck can fit the shipment
    if (!canFitShipment(truck, shipment)) {
        return 0;
    }

    // Check if truck has space for more shipments
    if (truck->numShipments >= MAX_SHIPMENTS) {
        return 0;
    }

    // Add shipment to cargo array
    truck->cargo[truck->numShipments] = *shipment;
    truck->numShipments++;

    // Update truck's current weight and volume
    truck->currentWeight += shipment->weight;
    truck->currentVolume += shipment->volume;

    return 1;
}

/*
* Name: assignShipment
* Author: Judd niemi
* Description: Places the shipment in the truck array using the project's set rules
* FULLY IMPLEMENTED
*/
struct DeliveryResult assignShipment(struct Truck trucks[], int numTrucks, const struct Shipment* s, const struct Map* map) {
    struct DeliveryResult result = { 0, -1, 0, 0.0 };

    if (trucks == NULL || numTrucks <= 0 || s == NULL || map == NULL) {
        return result;  // failure by default
    }

    double bestDiversionDist = 999999.0;
    double bestCapacityPercent = -1.0;

    for (int i = 0; i < numTrucks; i++) {
        // Can truck reach destination? Calculate diversion distance
        double diversionDist = calculateRouteDistance(&trucks[i], s->destination, map);

        if (diversionDist < 0) {
            // Can't reach destination, skip this truck
            continue;
        }

        // Can shipment fit in truck (weight + volume)?
        if (!canFitShipment(&trucks[i], s)) {
            continue;
        }

        // Calculate % capacity left (by weight or volume, whichever is less)
        double weightLeftPercent = (MAX_WEIGHT - trucks[i].currentWeight) / MAX_WEIGHT;
        double volumeLeftPercent = (MAX_VOLUME - trucks[i].currentVolume) / MAX_VOLUME;
        double capacityLeftPercent = (weightLeftPercent < volumeLeftPercent) ? weightLeftPercent : volumeLeftPercent;

        // Check if this truck is better by MS4 rules
        int isBetterTruck = 0;

        if (diversionDist < bestDiversionDist) {
            isBetterTruck = 1;
        }
        else if (diversionDist == bestDiversionDist) {
            if (capacityLeftPercent > bestCapacityPercent) {
                isBetterTruck = 1;
            }
            else if (capacityLeftPercent == bestCapacityPercent) {
                if (trucks[i].truckNumber < (result.truckIndex >= 0 ? trucks[result.truckIndex].truckNumber : 3)) {
                    isBetterTruck = 1;
                }
            }
        }

        if (isBetterTruck) {
            bestDiversionDist = diversionDist;
            bestCapacityPercent = capacityLeftPercent;
            result.truckIndex = i;
            result.distanceToGo = diversionDist;
            result.needsDiversion = (diversionDist > 0.01) ? 1 : 0;  // small epsilon for floating point
        }
    }

    if (result.truckIndex == -1) {
        // No truck could fit or reach destination
        return result;
    }

    // Try to add shipment to the selected truck
    if (addShipmentToTruck(&trucks[result.truckIndex], s)) {
        result.success = 1;
    }

    return result;
}

int colLetterToIndex(char letter) {
    if (letter >= 'a' && letter <= 'z') {
        letter -= 'a' - 'A'; // convert to uppercase
    }
    if (letter >= 'A' && letter <= 'Z') {
        int index = letter - 'A';
        if (index >= 0 && index < 25) {
            return index;
        }
    }
    return -1;  // invalid column
}

int parseDestination(const char* destStr, struct Point* point) {
    if (!destStr || !point) return 0;

    int len = (int)strlen(destStr);
    if (len < 2 || len > 3) return 0;  // row can be 1 or 2 digits

    // Extract row number substring (all except last char)
    char rowStr[3] = { 0 };
    strncpy(rowStr, destStr, len - 1);
    int row = atoi(rowStr);

    char colChar = destStr[len - 1];
    int col = colLetterToIndex(colChar);

    // Validate row and col are in bounds
    if (row < 0 || row >= 25 || col < 0 || col >= 25) {
        return 0;
    }

    point->row = row;
    point->col = col;
    return 1;
}

void printDeliveryInfo(const struct Truck* truck, const struct DeliveryResult* result, const struct Map* map, const struct Point* shipmentDestination) {
    const char* colors[] = { "BLUE", "GREEN", "YELLOW" };
    const char* color = "UNKNOWN";

    if (truck->truckNumber >= 0 && truck->truckNumber < 3) {
        color = colors[truck->truckNumber];
    }

    printf("truckNumber=%d, needsDiversion=%d, color=%s\n", truck->truckNumber, result->needsDiversion, color);

    if (!result->needsDiversion) {
        printf("Ship on %s LINE, no diversion\n", color);
    }
    else {
        printf("Ship on %s LINE, divert: ", color);
        // rest of your diversion printing code...
    }
}



int isValidDestination(const struct Point* dest, const struct Map* map) {
    if (!dest || !map) return 0;

    if (dest->row < 0 || dest->row >= map->numRows || dest->col < 0 || dest->col >= map->numCols) {
        return 0;
    }

    int cellValue = map->squares[dest->row][dest->col];

    if (cellValue == 'B') {  // or whatever value indicates blocked
        return 0;
    }

    return 1;
}




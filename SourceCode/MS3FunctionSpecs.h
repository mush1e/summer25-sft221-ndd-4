#ifndef MS3FUNCTIONSPECS_H
#define MS3FUNCTIONSPECS_H

#include "delivery.h"
#include "mapping.h"

/*
* Name: remainingCapacityKg
* Author: Mustafa Siddiqui
* Description: Returns the remaining weight capacity (kg) for the given truck
* Parameters:
*   - t: pointer to the truck structure
* Returns: Integer representing remaining weight capacity in kg, 0 if truck is NULL or at capacity
*/
int remainingCapacityKg(const struct Truck* t);

/*
* Name: remainingVolumeM3
* Author: Mustafa Siddiqui
* Description: Returns the remaining volume capacity (in cubic metres) for the selected truck
* Parameters:
*   - t: pointer to the truck structure
* Returns: Double representing remaining volume capacity in cubic meters, 0.0 if truck is NULL or at capacity
*/
double remainingVolumeM3(const struct Truck* t);

/*
* Name: canFitShipment
* Author: Mustafa Siddiqui
* Description: Checks whether a shipment will be able to fit into specified truck
* Parameters:
*   - t: pointer to the truck structure
*   - s: pointer to the shipment structure
* Returns: 1 if shipment can fit (both weight and volume), 0 otherwise
*/
int canFitShipment(const struct Truck* t, const struct Shipment* s);

/*
* Name: assignShipment
* Author: Mustafa Siddiqui
* Description: Places the shipment in the truck array using the project's set rules.
*              Finds the best truck for a shipment considering load, route proximity,
*              and capacity. Returns the index of the selected truck.
* Parameters:
*   - trucks: array of truck structures
*   - numTrucks: number of trucks in the array
*   - s: pointer to the shipment to be assigned
* Returns: Index of the truck (0-2) where shipment was assigned, or -1 if no truck can take it
*/
struct DeliveryResult assignShipment(struct Truck trucks[], int numTrucks, const struct Shipment* s, const struct Map* map);


/*
* Name: findClosestTruck
* Author: Mustafa Siddiqui
* Description: Finds the truck that comes closest to the destination point.
*              Uses Euclidean distance to determine the nearest point on each truck's route.
* Parameters:
*   - trucks: array of truck structures
*   - numTrucks: number of trucks in the array
*   - destination: the destination point for delivery
*   - map: the map containing building information
* Returns: Index of the closest truck (0-2), or -1 if no valid route exists
*/
int findClosestTruck(const struct Truck trucks[], int numTrucks, const struct Point destination, const struct Map* map);

/*
* Name: calculateRouteDistance
* Author: Mustafa Siddiqui
* Description: Calculates the shortest distance from a truck's route to a destination point.
*              Uses the A* algorithm to find the shortest path avoiding buildings.
* Parameters:
*   - truck: pointer to the truck structure
*   - destination: the destination point
*   - map: the map containing building information
* Returns: Distance to destination, or -1.0 if destination is unreachable
*/
double calculateRouteDistance(const struct Truck* truck, const struct Point destination, const struct Map* map);

/*
* Name: addShipmentToTruck
* Author: Mustafa Siddiqui
* Description: Adds a shipment to the specified truck's cargo array and updates
*              the truck's current weight and volume.
* Parameters:
*   - truck: pointer to the truck structure
*   - shipment: pointer to the shipment to be added
* Returns: 1 if shipment was successfully added, 0 otherwise
*/

int colLetterToIndex(char letter);
int isValidDestination(const struct Point* dest, const struct Map* map);
void printDeliveryInfo(const struct Truck* truck, const struct DeliveryResult* result, const struct Map* map, const struct Point* shipmentDestination);


#endif
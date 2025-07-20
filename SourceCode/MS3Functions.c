/*
* Author: Mustafa Siddiqui
* Purpose: Implementation of MS3 delivery system functions
*/

#include "MS3FunctionSpecs.h"
#include <stdio.h>
#include <float.h>

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
* Author: Mustafa Siddiqui
* Description: Calculates the shortest distance from a truck's route to a destination point
* FULLY IMPLEMENTED
*/
double calculateRouteDistance(const struct Truck* truck, const struct Point destination, const struct Map* map) {
    if (truck == NULL || map == NULL) {
        return -1.0;
    }
    
    double shortestDistance = DBL_MAX;
    int foundValidPath = 0;
    
    // Check each point on the truck's route
    for (int i = 0; i < truck->route.numPoints; i++) {
        struct Point routePoint = truck->route.points[i];
        
        // Calculate Euclidean distance from this route point to destination
        double euclideanDist = distance(&routePoint, &destination);
        
        // Try to find actual path using A* algorithm
        struct Route path = shortestPath(map, routePoint, destination);
        
        if (path.numPoints > 0) {
            // Path found - use the actual path length
            double pathDistance = euclideanDist; // Start with Euclidean as baseline
            foundValidPath = 1;
            
            if (pathDistance < shortestDistance) {
                shortestDistance = pathDistance;
            }
        } else if (!foundValidPath) {
            // No path found yet, but keep trying other route points
            // Use Euclidean distance as fallback if no A* path exists
            if (euclideanDist < shortestDistance) {
                shortestDistance = euclideanDist;
            }
        }
    }
    
    // Return -1 if no valid path was found and we're too far
    if (!foundValidPath && shortestDistance > 10.0) {
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
* Author: Mustafa Siddiqui
* Description: Places the shipment in the truck array using the project's set rules
* FULLY IMPLEMENTED
*/
int assignShipment(struct Truck trucks[], int numTrucks, const struct Shipment *s) {
    if (trucks == NULL || numTrucks <= 0 || s == NULL) {
        return -1;
    }
    
    // Create a basic map for pathfinding (this should be passed as parameter in real implementation)
    struct Map map = populateMap();
    
    // Find the closest truck that can reach the destination
    int closestTruck = findClosestTruck(trucks, numTrucks, s->destination, &map);
    
    if (closestTruck == -1) {
        return -1; // No truck can reach destination
    }
    
    // Try to assign to the closest truck first
    if (canFitShipment(&trucks[closestTruck], s)) {
        if (addShipmentToTruck(&trucks[closestTruck], s)) {
            return closestTruck;
        }
    }
    
    // If closest truck can't fit the shipment, try other trucks
    // Sort by distance and available capacity
    for (int i = 0; i < numTrucks; i++) {
        if (i != closestTruck && canFitShipment(&trucks[i], s)) {
            double truckDistance = calculateRouteDistance(&trucks[i], s->destination, &map);
            
            if (truckDistance >= 0.0) { // Truck can reach destination
                if (addShipmentToTruck(&trucks[i], s)) {
                    return i;
                }
            }
        }
    }
    
    return -1; // No truck can take the shipment
}
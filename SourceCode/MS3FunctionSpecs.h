#ifndef MS3FUNCTIONSPECS_H
#define MS3FUNCTIONSPECS_H

#include "delivery.h"   

/*Name:  remainingCapacityKg                                                

  Description: 
  Returns the remaining weight capacity (kg) for the given     
  truck.  If the truck is already overweight, the function returns 0 to     
  signal no additional cargo can be loaded. 

  Parameters:                                                               
   t – pointer to a Truck where the currentWeightKg is being 
   maintained.

  Returns:  
  The number of kilograms that can still be loaded. If t is NULL,
  or currentWeight >= MAX_WEIGHT, the function returns 0.
  */

int remainingCapacityKg(const struct Truck *t);

/*Name:  remainingVolumeM3

  Description: 
  Returns the remaining volume capacity (in cubic metres) for the
  selected truck.

  Parameters:
   t – pointer to a Truck whose currentVolume field is
   up to date.

  Returns:  
  Cubic meters (as a double) still available. If t has no more space 
  left or is NULL, 0.0 will be returned.
  */

double remainingVolumeM3(const struct Truck *t);

/*Name:  canFitShipment

  Description:
  Checks whether a shipment will be able to fit into specified truck
  based on volume and weight limits.

  Parameters:
   t – pointer to Truck.
   s – pointer to Shiptment.

  Returns:
  1 (True) if shipment weight is below the remainging capacity of the 
  Truck and shipment volume is below the remaining volume of the Truck.
  Otherwise, it returns 0.
  */                              
int canFitShipment(const struct Truck *t, const struct Shipment *s);

/*Name:  assignShipment

  Description:
  Places the shipment in the truck array using the projects set rules 
  provided.

  Parameters:
   trucks – array of Truck structures.
   numTrucks - length of trucks array.
   s - pointer to shipment to be placed.

  Returns:
  Index of the chosen truck upon the success of finding one. -1 if no truck
  can take the shipment.
  */
int assignShipment(struct Truck trucks[], int numTrucks, const struct Shipment *s);

#endif 

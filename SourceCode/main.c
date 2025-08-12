
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "mapping.h"
#include "delivery.h"
#include "MS3FunctionSpecs.h"

int main() {
    struct Map baseMap = populateMap();
    struct Truck trucks[NUM_TRUCKS] = { 0 };

    trucks[0].route = getBlueRoute();
    trucks[0].truckNumber = 0;
    trucks[1].route = getGreenRoute();
    trucks[1].truckNumber = 1;
    trucks[2].route = getYellowRoute();
    trucks[2].truckNumber = 2;

    for (int i = 0; i < NUM_TRUCKS; i++) {
        trucks[i].numShipments = 0;
        trucks[i].currentWeight = 0.0;
        trucks[i].currentVolume = 0.0;
    }

    printf("=================\nSeneca Polytechnic Deliveries:\n=================\n");

    while (1) {
        double weight;
        double volume;
        char destinationStr[4];  
        printf("Enter shipment weight, box size and destination (0 0 x to stop): ");
        int inputs = scanf("%lf %lf %3s", &weight, &volume, destinationStr);

        if (inputs != 3) {
            while (getchar() != '\n'); 
            printf("Invalid input\n");
            continue;
        }

        if (weight == 0 && volume == 0 && (destinationStr[0] == 'x' || destinationStr[0] == 'X')) {
            printf("Thanks for shipping with Seneca Polytechnic!\n");
            break;
        }

        if (weight < 1 || weight > 5000) {
            printf("Invalid weight (must be 1-5000 Kg.)\n");
            continue;
        }

        if (volume != 0.5 && volume != 2.0 && volume != 5.0) {
            printf("Invalid size\n");
            continue;
        }

        struct Point destination;
        if (!parseDestination(destinationStr, &destination)) {
            printf("Invalid destination\n");
            continue;
        }

        if (!isValidDestination(&destination, &baseMap)) {
            printf("Invalid destination\n");
            continue;
        }

        struct Shipment shipment = { weight, volume, destination };

        struct DeliveryResult result = assignShipment(trucks, NUM_TRUCKS, &shipment, &baseMap);

        if (result.success) {
            printDeliveryInfo(&trucks[result.truckIndex], &result, &baseMap, &shipment.destination);
        }
        else {
            printf("Ships tomorrow\n");
        }
    }

    return 0;
}

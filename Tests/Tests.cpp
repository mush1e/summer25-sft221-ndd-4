#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../FunctionSpecs/MS3FunctionSpecs.h"
#include "../SourceCode/delivery.h"
#include "../SourceCode/mapping.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// -------------------------------------------------------------
//  remainingCapacityKg  (BB‑001 .. BB‑004)
// -------------------------------------------------------------

TEST_CLASS(BB_RemainingCapacityKg)
{
public:
    TEST_METHOD(BB_001_EmptyTruck)
    {
        struct Truck t = { 0 };                  
        Assert::AreEqual(MAX_WEIGHT, remainingCapacityKg(&t));
    }
    TEST_METHOD(BB_002_HalfLoad)
    {
        struct Truck t = { 0 };
        t.currentWeight = MAX_WEIGHT / 2;
        Assert::AreEqual(MAX_WEIGHT / 2, remainingCapacityKg(&t));
    }
    TEST_METHOD(BB_003_OverWeight)
    {
        struct Truck t = { 0 };
        t.currentWeight = MAX_WEIGHT + 1;
        Assert::AreEqual(0, remainingCapacityKg(&t));
    }
    TEST_METHOD(BB_004_NullPtr)
    {
        Assert::AreEqual(0, remainingCapacityKg(nullptr));
    }
};

/* remainingVolumeM3   (BB‑005 … BB‑008) */

TEST_CLASS(BB_RemainingVolumeM3)
{
public:
    TEST_METHOD(BB_005_EmptyTruck)
    {
        struct Truck t = { 0 };
        Assert::AreEqual(static_cast<double>(MAX_VOLUME),
            remainingVolumeM3(&t),
            0.0001);
    }
    TEST_METHOD(BB_006_QuarterLeft)
    {
        struct Truck t = { 0 };
        t.currentVolume = 0.75 * MAX_VOLUME;
        Assert::AreEqual((int)(0.25 * MAX_VOLUME),
            (int)remainingVolumeM3(&t));
    }
    TEST_METHOD(BB_007_VolumeOverflow)
    {
        struct Truck t = { 0 };
        t.currentVolume = MAX_VOLUME + 1;
        Assert::AreEqual(0.0, remainingVolumeM3(&t), 0.0001);
    }
    TEST_METHOD(BB_008_NullPtr)
    {
        Assert::AreEqual(0.0, remainingVolumeM3(nullptr), 0.0001);
    }
};

/* canFitShipment   (BB‑009 … BB‑012)  */

TEST_CLASS(BB_CanFitShipment)
{
public:
    TEST_METHOD(BB_009_FitsBoth)
    {
        struct Truck t = { 0 };
        struct Shipment s = { 10, 1, {0,0} };
        Assert::IsTrue(canFitShipment(&t, &s));
    }
    TEST_METHOD(BB_010_VolumeTooBig)
    {
        struct Truck t = { 0 };
        t.currentVolume = MAX_VOLUME - 1;    // almost full
        struct Shipment s = { 1, 2, {0,0} }; // 2 m³ won’t fit
        Assert::IsFalse(canFitShipment(&t, &s));
    }
    TEST_METHOD(BB_011_WeightTooHeavy)
    {
        struct Truck t = { 0 };
        t.currentWeight = MAX_WEIGHT - 1;    // almost at limit
        struct Shipment s = { 600, 1, {0,0} };
        Assert::IsFalse(canFitShipment(&t, &s));
    }
    TEST_METHOD(BB_012_NullPtrs)
    {
        Assert::IsFalse(canFitShipment(nullptr, nullptr));
    }
};

/* assignShipment   (BB‑013 … BB‑016) */

TEST_CLASS(BB_AssignShipment)
{
public:
    TEST_METHOD(BB_013_PicksFirstTruck)
    {
        struct Truck trucks[2] = { 0 };
        struct Shipment s = { 10, 1, {5,5} };

        // --- Fake a one‑point route so calculateRouteDistance returns 0 ---
        for (int i = 0; i < 2; ++i) {
            trucks[i].route.numPoints = 1;
            trucks[i].route.points[0] = s.destination;
        }

        int idx = assignShipment(trucks, 2, &s);
        Assert::AreEqual(0, idx);
    }

    TEST_METHOD(BB_014_PicksSecondTruck)
    {
        struct Truck trucks[2] = { 0 };
        struct Shipment s = { 10, 1, {5,5} };

        // fake routes
        for (int i = 0; i < 2; ++i) {
            trucks[i].route.numPoints = 1;
            trucks[i].route.points[0] = s.destination;
        }

        // fill truck 0 so it’s skipped
        trucks[0].currentWeight = MAX_WEIGHT;

        int idx = assignShipment(trucks, 2, &s);
        Assert::AreEqual(1, idx);
    }

    TEST_METHOD(BB_015_NoTruckFits)
    {
        struct Truck trucks[1] = { 0 };
        trucks[0].currentWeight = MAX_WEIGHT;
        struct Shipment s = { 10, 1, {5,5} };
        Assert::AreEqual(-1, assignShipment(trucks, 1, &s));
    }
    TEST_METHOD(BB_016_NullArray)
    {
        struct Shipment s = { 1, 1, {0,0} };
        Assert::AreEqual(-1, assignShipment(nullptr, 0, &s));
    }
};
TEST_CLASS(WB_RemainingCapacityKg)
{
public:
    TEST_METHOD(WBT_001_RemCap_2000)
    {
        struct Truck t = { 0 };
        t.currentWeight = 2000;
        Assert::AreEqual(3000, remainingCapacityKg(&t));
    }

    TEST_METHOD(WBT_002_RemCap_AtCapacity)
    {
        struct Truck t = { 0 };
        t.currentWeight = 10000;
        Assert::AreEqual(0, remainingCapacityKg(&t));
    }

    TEST_METHOD(WBT_003_RemCap_OverCapacity)
    {
        struct Truck t = { 0 };
        t.currentWeight = 10500;
        Assert::AreEqual(0, remainingCapacityKg(&t));
    }

    TEST_METHOD(WBT_004_RemCap_NullPtr)
    {
        Assert::AreEqual(0, remainingCapacityKg(nullptr));
    }
};

TEST_CLASS(WB_RemainingVolumeM3)
{
public:
    TEST_METHOD(WBT_005_RemVol_30)
    {
        struct Truck t = { 0 };
        t.currentVolume = 30.0;
        Assert::AreEqual(170.0, remainingVolumeM3(&t), 0.0001);
    }

    TEST_METHOD(WBT_006_RemVol_AtCapacity)
    {
        struct Truck t = { 0 };
        t.currentVolume = 200.0;
        Assert::AreEqual(0.0, remainingVolumeM3(&t), 0.0001);
    }

    TEST_METHOD(WBT_007_RemVol_OverCapacity)
    {
        struct Truck t = { 0 };
        t.currentVolume = 220.0;
        Assert::AreEqual(0.0, remainingVolumeM3(&t), 0.0001);
    }

    TEST_METHOD(WBT_008_RemVol_NullPtr)
    {
        Assert::AreEqual(0.0, remainingVolumeM3(nullptr), 0.0001);
    }
};

TEST_CLASS(WB_CanFitShipment)
{
public:
    TEST_METHOD(WBT_009_FitsBoth)
    {
        struct Truck t = { 0 };
        t.currentWeight = 2000;  // 8000kg remaining
        t.currentVolume = 30.0;  // 70m³ remaining
        struct Shipment s = { 500, 5.0, {0,0} };
        Assert::IsTrue(canFitShipment(&t, &s));
    }

    TEST_METHOD(WBT_010_NullTruck)
    {
        struct Shipment s = { 500, 5.0, {0,0} };
        Assert::IsFalse(canFitShipment(nullptr, &s));
    }

    TEST_METHOD(WBT_011_NullShipment)
    {
        struct Truck t = { 0 };
        Assert::IsFalse(canFitShipment(&t, nullptr));
    }

    TEST_METHOD(WBT_012_ExceedsWeight)
    {
        struct Truck t = { 0 };
        t.currentWeight = 9000;  // 1000kg remaining
        struct Shipment s = { 1500, 5.0, {0,0} };  // Exceeds remaining weight
        Assert::IsFalse(canFitShipment(&t, &s));
    }
};

TEST_CLASS(WB_AddShipmentToTruck)
{
public:
    TEST_METHOD(WBT_013_Add_Valid)
    {
        struct Truck t = { 0 };
        struct Shipment s = { 500, 5.0, {0,0} };
        Assert::AreEqual(1, addShipmentToTruck(&t, &s));
    }

    TEST_METHOD(WBT_014_Add_FullArray)
    {
        struct Truck t = { 0 };
        t.numShipments = MAX_SHIPMENTS;
        struct Shipment s = { 1, 1.0, {0,0} };
        Assert::AreEqual(0, addShipmentToTruck(&t, &s));
    }

    TEST_METHOD(WBT_015_Add_TooLarge)
    {
        struct Truck t = { 0 };
        t.currentWeight = 9000;  // 1000kg remaining
        struct Shipment s = { 1500, 5.0, {0,0} };  // Too heavy
        Assert::AreEqual(0, addShipmentToTruck(&t, &s));
    }

    TEST_METHOD(WBT_016_Add_NullTruck)
    {
        struct Shipment s = { 500, 5.0, {0,0} };
        Assert::AreEqual(0, addShipmentToTruck(nullptr, &s));
    }
};

TEST_CLASS(WB_AssignShipment)
{
public:
    TEST_METHOD(WBT_017_Assign_Valid)
    {
        struct Map map = populateMap();
        struct Truck trucks[2] = { 0 };
        struct Shipment s = { 10, 2.0, {1,1} };

        // Fake a one-point route so calculateRouteDistance returns 0
        for (int i = 0; i < 2; ++i) {
            trucks[i].route.numPoints = 1;
            trucks[i].route.points[0] = s.destination;
        }

        // Fill truck 0 so it's at capacity and skipped
        trucks[0].currentWeight = MAX_WEIGHT;

        int idx = assignShipment(trucks, 2, &s);
        Assert::AreEqual(1, idx);
    }

    TEST_METHOD(WBT_018_Assign_AllFull)
    {
        struct Map map = populateMap();
        struct Truck trucks[2] = { 0 };
        // Setup all trucks at capacity
        for (int i = 0; i < 2; i++) {
            trucks[i].currentWeight = 10000;
            trucks[i].currentVolume = 100.0;
            trucks[i].route.numPoints = 1;
            trucks[i].route.points[0] = { 0,0 };
        }

        struct Shipment s = { 1, 1.0, {0,0} };
        Assert::AreEqual(-1, assignShipment(trucks, 2, &s));
    }

    TEST_METHOD(WBT_019_Assign_NullArray)
    {
        struct Shipment s = { 500, 5.0, {0,0} };
        Assert::AreEqual(-1, assignShipment(nullptr, 0, &s));
    }

    TEST_METHOD(WBT_020_Assign_NullShipment)
    {
        struct Map map = populateMap();
        struct Truck trucks[1] = { 0 };
        trucks[0].route.numPoints = 1;
        trucks[0].route.points[0] = { 0,0 };
        Assert::AreEqual(-1, assignShipment(trucks, 1, nullptr));
    }
};

TEST_CLASS(WB_FindClosestTruck)
{
public:
    TEST_METHOD(WBT_021_FindClosest_Valid)
    {
        struct Map map = populateMap();
        struct Truck trucks[3] = { 0 };
        // Set different positions for trucks
        trucks[0].route.numPoints = 1;
        trucks[0].route.points[0] = { 1,1 };
        trucks[1].route.numPoints = 1;
        trucks[1].route.points[0] = { 5,5 };
        trucks[2].route.numPoints = 1;
        trucks[2].route.points[0] = { 10,10 };

        struct Point destination = { 4,4 };
        // Truck 1 at (5,5) should be closest to (4,4)
        Assert::AreEqual(1, findClosestTruck(trucks, 3, destination, &map));
    }

    TEST_METHOD(WBT_022_FirstTruckValid)
    {
        // Arrange
        struct Map map = populateMap();
        struct Truck trucks[2] = { 0 };
        struct Point dest = { 10, 10 };

        // Make truck 0 reachable
        trucks[0].route.numPoints = 1;
        trucks[0].route.points[0] = dest;

        // Leave truck 1 unreachable
        trucks[1].route.numPoints = 0;

        // Act
        int idx = findClosestTruck(trucks, 2, dest, &map);

        // Assert
        Assert::AreEqual(0, idx);
    }



    TEST_METHOD(WBT_023_FindClosest_NullMap)
    {
        struct Truck trucks[1] = { 0 };
        trucks[0].route.numPoints = 1;
        trucks[0].route.points[0] = { 0,0 };

        struct Point destination = { 1,1 };
        Assert::AreEqual(-1, findClosestTruck(trucks, 1, destination, nullptr));
    }
};

TEST_CLASS(WB_CalculateRouteDistance)
{
public:
    TEST_METHOD(WBT_024_CalcDist_Valid)
    {
        struct Map map = populateMap();
        struct Truck t = { 0 };
        t.route.numPoints = 1;
        t.route.points[0] = { 0,0 };

        struct Point dest = { 1,1 };
        double distance = calculateRouteDistance(&t, dest, &map);
        Assert::IsTrue(distance >= 0.0);
    }

    TEST_METHOD(WBT_025_CalcDist_Euclidean)
    {
        struct Map map = populateMap();
        struct Truck t = { 0 };
        t.route.numPoints = 1;
        t.route.points[0] = { 0,0 };

        struct Point dest = { 3,4 };  
        double distance = calculateRouteDistance(&t, dest, &map);
        Assert::AreEqual(5.0, distance, 0.0001);
    }

    TEST_METHOD(WBT_026_CalcDist_NullTruck)
    {
        struct Map map = populateMap();
        struct Point dest = { 0,0 };
        Assert::AreEqual(-1.0, calculateRouteDistance(nullptr, dest, &map), 0.0001);
    }

    TEST_METHOD(WBT_027_CalcDist_NullMap)
    {
        struct Truck t = { 0 };
        t.route.numPoints = 1;
        t.route.points[0] = { 0,0 };

        struct Point dest = { 1,1 };
        Assert::AreEqual(-1.0, calculateRouteDistance(&t, dest, nullptr), 0.0001);
    }
};
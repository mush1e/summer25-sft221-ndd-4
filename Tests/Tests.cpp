#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../FunctionSpecs/MS3FunctionSpecs.h"
#include "../SourceCode/delivery.h"
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
        struct Truck trucks[2] = { 0 };              // both empty
        struct Shipment s = { 10, 1, {5,5} };
        Assert::AreEqual(0, assignShipment(trucks, 2, &s));
    }
    TEST_METHOD(BB_014_PicksSecondTruck)
    {
        struct Truck trucks[2] = { 0 };
        trucks[0].currentWeight = MAX_WEIGHT;        // first full
        struct Shipment s = { 10, 1, {5,5} };
        Assert::AreEqual(1, assignShipment(trucks, 2, &s));
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
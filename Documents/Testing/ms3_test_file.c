#include "pch.h"
extern "C" {
#include "MS3FunctionSpecs.h"
#include "delivery.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
    TEST_CLASS(MS3FunctionTests)
    {
    public:
        TEST_METHOD(TestRemainingCapacityKg)
        {
            Truck t = {};
            t.currentWeight = 3000;
            Assert::AreEqual(2000, remainingCapacityKg(&t));

            t.currentWeight = 5200;
            Assert::AreEqual(0, remainingCapacityKg(&t));

            t.currentWeight = 0;
            Assert::AreEqual(5000, remainingCapacityKg(&t));

            Truck* nullTruck = nullptr;
            Assert::AreEqual(0, remainingCapacityKg(nullTruck));
        }

        TEST_METHOD(TestRemainingVolumeM3)
        {
            Truck t = {};
            t.currentVolume = 100.0;
            Assert::AreEqual(100.0, remainingVolumeM3(&t), 0.0001);

            t.currentVolume = 200.0;
            Assert::AreEqual(0.0, remainingVolumeM3(&t), 0.0001);

            t.currentVolume = 210.0;
            Assert::AreEqual(0.0, remainingVolumeM3(&t), 0.0001);

            Truck* nullTruck = nullptr;
            Assert::AreEqual(0.0, remainingVolumeM3(nullTruck), 0.0001);
        }

        TEST_METHOD(TestCanFitShipment)
        {
            Truck t = {};
            t.currentWeight = 4000;
            t.currentVolume = 100.0;

            Shipment s = { 500, 50.0, {5, 11} };  // 'K' converted to column index 11

            Assert::IsTrue(canFitShipment(&t, &s));

            s.weight = 1500;
            Assert::IsFalse(canFitShipment(&t, &s));  // Too heavy

            s.weight = 500;
            s.volume = 120.0;
            Assert::IsFalse(canFitShipment(&t, &s));  // Too large

            Assert::IsFalse(canFitShipment(nullptr, &s));
            Assert::IsFalse(canFitShipment(&t, nullptr));
        }

        TEST_METHOD(TestAssignShipmentStub)
        {
            Truck trucks[3] = {};
            trucks[0].currentWeight = 1000;
            trucks[0].currentVolume = 50;
            trucks[1].currentWeight = 2000;
            trucks[1].currentVolume = 100;
            trucks[2].currentWeight = 3000;
            trucks[2].currentVolume = 150;

            Shipment s = { 500, 50.0, {7, 5} };  // 'F' converted to column index 5

            int result = assignShipment(trucks, 3, &s);
            Assert::AreEqual(-1, result);  // Stub always returns -1
        }
    };
}

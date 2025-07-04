#pragma once

#include "raylib.h"
#include <vector>
#include "Vehicle.hpp"
#include "Game/World/World.hpp"

class Vehicle;
class TrafficGenerator;

class TrafficManager {
    private:
        World* worldHandler;
        std::vector<Vehicle*> vehicles;
        float tickRate = 0.1f; // 10 ticks per second
        float tickAccumulator = 0.0f;
    public:
        TrafficManager(World* wh) : worldHandler(wh) {};
        void Tick(float deltaTime);
        void SpawnVehicle(TrafficGenerator* generator);
        void SpawnVehicleAtCoord(Vector3 position);
        void UpdateVehicles();
        void RemoveArrivedVehicles();
        void RecalculateDirection(Vehicle* vehicle);
        void RenderVehicle();
        void DespawnVehicle(Vehicle*& veh);
};

#include "TrafficManager.hpp"

void TrafficManager::SpawnVehicleAtCoord(Vector3 position) {
    RoadSegment* road = worldHandler->FindNearestRoad(position);
    if (!road) return;
    
    Vehicle* veh = new Vehicle(position, {0, 0, 0}); //test destination
    vehicles.push_back(veh);
    

    veh->currentRoad = road;
    veh->nextNode = road->end;
    veh->RecalculateDirection();
}

void TrafficManager::Tick(float deltaTime) {
    UpdateVehicles();
    RenderVehicle();
    
}

void TrafficManager::UpdateVehicles() {
    std::vector<Vehicle*> vehiclesToDelete;

    for (auto& veh : vehicles) {
        veh->GoForward();

        float goalDist = Vector3Distance(veh->goalPosition, veh->currentPosition);
        if (std::abs(goalDist) < 4.0f) {vehiclesToDelete.push_back(veh); continue;}
        
        float minDistance = 999999.9;
        if (auto node = worldHandler->FindNearestNode(veh->currentPosition, 4.0f)) {
            const std::vector<TrafficNode*>& connections = node->ShowNeighbours();
            // if (connections.size() == 2) {
                
            //     continue;
            // } 
            for (auto& nextN : connections) {
                
                float dist = Vector3Distance(nextN->GetWorldPosition(), veh->goalPosition);
                if (dist < minDistance) {
                    minDistance = dist;
                    veh->nextNode = nextN;
                }
            }
        }
        veh->RecalculateDirection();
    }

    for (auto& veh : vehiclesToDelete) {
        DespawnVehicle(veh);
        return;
    }

}

void TrafficManager::RenderVehicle() {
    for (auto& veh : vehicles) {
        DrawCube(veh->currentPosition, 4.0f, 4.0f, 6.0f, PURPLE);
    }
}

void TrafficManager::DespawnVehicle(Vehicle*& veh) {
    vehicles.erase(
        std::remove(vehicles.begin(), vehicles.end(), veh),
        vehicles.end()
    );  
    delete veh;
}

TrafficManager::~TrafficManager() {
    for (auto veh : vehicles) {
        delete veh;
    }
}
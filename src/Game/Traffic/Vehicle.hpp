#pragma once

#include "raylib.h"
#include "TrafficNode.hpp"
#include "TrafficManager.hpp"

class Vehicle {
    private:
        Vector3 currentPosition;
        Vector3 goalPosition;
        TrafficNode* nextNode;

        RoadSegment* currentRoad;
        Vector3 direction; //recalculate on each road;

        float speed;
    public:
        friend class TrafficManager;
        Vehicle(Vector3 position, Vector3 destination, float speed = 2.0) :
        currentPosition(position), goalPosition(destination), speed(speed) {};
        void RecalculateDirection();
        void GoForward();
};

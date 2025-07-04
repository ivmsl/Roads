#include "Vehicle.hpp"

void Vehicle::RecalculateDirection() {
    if (!nextNode) return;
    direction = Vector3Normalize(Vector3Subtract(nextNode->GetWorldPosition(), currentPosition));
}

void Vehicle::GoForward() {
    currentPosition = Vector3Add(currentPosition, Vector3Scale(direction, speed * GetFrameTime()));
}
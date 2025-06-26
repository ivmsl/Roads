#include "raylib.h"
#include "raymath.h"
#include "CameraController.hpp"
#include <cmath>

inline void CameraController::RecalculatePosition() {
    float tiltRadians = tiltAngle * DEG2RAD;
    float sideRadians = sideTilt * DEG2RAD;
    
    float baseY = cameraDistance * sin(tiltRadians);
    float baseZ = cameraDistance * cos(tiltRadians);
    
    camera.target = targetPosition;
    camera.position = {
        targetPosition.x + baseZ * sin(sideRadians),    // X offset from side tilt
        targetPosition.y + baseY,                       // Y height (same as before)
        targetPosition.z - baseZ * cos(sideRadians)     // Z adjusted for side tilt
    };
}

void CameraController::Initialize() {
    // float tiltRadians = tiltAngle * DEG2RAD;
    
    camera = {};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.up = {0.0f, 1.0f, 0.0f}; //Y — UP - Second

    RecalculatePosition();
    camera.fovy = 65.0f;

// Debug output
    // TraceLog(LOG_INFO, "Tilt angle: %.2f degrees (%.2f radians)", tiltAngle, tiltRadians);
    // TraceLog(LOG_INFO, "sin(%.2f) = %.3f, cos(%.2f) = %.3f", tiltAngle, sin(tiltRadians), tiltAngle, cos(tiltRadians));
    // TraceLog(LOG_INFO, "Target: (%.2f, %.2f, %.2f)", targetPosition.x, targetPosition.y, targetPosition.z);
    // TraceLog(LOG_INFO, "Position: (%.2f, %.2f, %.2f)", camera.position.x, camera.position.y, camera.position.z);

}

Camera3D& CameraController::GetCamera() {
    return camera;
}



void CameraController::Update() {

}

void CameraController::Pan(Vector2 movementDelta) {
        float panSpeed = 0.5f;
        targetPosition = camera.target;
        targetPosition.x += movementDelta.x * panSpeed;
        targetPosition.z += movementDelta.y * panSpeed;
        
        RecalculatePosition();
}

void CameraController::Zoom(float wheelDelta) {
    cameraDistance += wheelDelta * -5.0f;
    cameraDistance = Clamp(cameraDistance, 20.0f, 100.0f);    
    RecalculatePosition();
}

void CameraController::Rotate(float tiltFix) {
    sideTilt += tiltFix;
    RecalculatePosition();
}
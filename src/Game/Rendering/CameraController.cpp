#include "raylib.h"
#include "raymath.h"
#include "CameraController.hpp"
#include <cmath>

inline void CameraController::RecalculateTilt() {
    float tiltRadians = tiltAngle * DEG2RAD;
    float sideRadians = sideTilt * DEG2RAD;
    
    float baseY = cameraDistance * sin(tiltRadians);
    float baseZ = cameraDistance * cos(tiltRadians);
    
    camera.position = {
        targetPosition.x + baseZ * sin(sideRadians),    // X offset from side tilt
        targetPosition.y + baseY,                       // Y height (same as before)
        targetPosition.z - baseZ * cos(sideRadians)     // Z adjusted for side tilt
    };
}

void CameraController::Initialize() {
    float tiltRadians = tiltAngle * DEG2RAD;
    
    camera = {0};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.target = targetPosition;
    camera.up = {0.0f, 1.0f, 0.0f}; //Y — UP - Second

    RecalculateTilt();

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
    Vector2 movementDelta = {0.0f, 0.0f};
    float panSpeed = 0.5f;
    bool shouldMove = false;
    
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        movementDelta = GetMouseDelta();
        shouldMove = true;
    }

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
        float keyboardSpeed = 2.0f; 
        
        if (IsKeyDown(KEY_W)) movementDelta.y += keyboardSpeed;  // Move forward
        if (IsKeyDown(KEY_S)) movementDelta.y -= keyboardSpeed;  // Move backward  
        if (IsKeyDown(KEY_A)) movementDelta.x += keyboardSpeed;  // Move left
        if (IsKeyDown(KEY_D)) movementDelta.x -= keyboardSpeed;  // Move right
        
        shouldMove = true;
        // TraceLog(LOG_INFO, "Keyboard Delta: %.2f, %.2f", movementDelta.x, movementDelta.y);
    }

    if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_E)) {
        float keyboardSpeed = 2.0f;
        if (IsKeyDown(KEY_Q)) sideTilt += keyboardSpeed;
        if (IsKeyDown(KEY_E)) sideTilt -= keyboardSpeed;
        shouldMove = true;

    }
    

    if (shouldMove) {
        targetPosition = camera.target;
        targetPosition.x += movementDelta.x * panSpeed;
        targetPosition.z += movementDelta.y * panSpeed;
        camera.target = targetPosition;
        RecalculateTilt();

    }
    
    // Handle zoom (mouse wheel)
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        cameraDistance += wheelMove * -5.0f;
        cameraDistance = Clamp(cameraDistance, 20.0f, 100.0f);
        
        RecalculateTilt();
    }
}
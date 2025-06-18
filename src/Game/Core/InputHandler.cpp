#include "raylib.h"
#include "raymath.h"
#include "InputHandler.hpp"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"

void InputHandler::Initialize(CameraController* camera, GridRenderer* grid) {
    cameraController = camera;
    gridRenderer = grid;
}

void InputHandler::ProcessInput() {
    HandleCameraInput();
}

void InputHandler::HandleCameraInput() {
    Vector2 movementDelta = {0.0f, 0.0f};
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
    }

    if (shouldMove) {
        cameraController->Pan(movementDelta);
    }

    if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_E)) {
        float keyboardSpeed = 2.0f;
        if (IsKeyDown(KEY_Q)) cameraController->Rotate(keyboardSpeed);
        if (IsKeyDown(KEY_E)) cameraController->Rotate(-keyboardSpeed);
        
    }

    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        cameraController->Zoom(wheelMove);
    }


}
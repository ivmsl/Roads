#include "raylib.h"
#include "raymath.h"
#include "InputHandler.hpp"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"

void InputHandler::Initialize(CameraController* camera, GridRenderer* grid, UIManager* uim) {
    cameraController = camera;
    gridRenderer = grid;
    uiManager = uim;
}

void InputHandler::ProcessInput() {
    HandleCameraInput();
    HandleRoadPlacement();
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
        
        if (IsKeyDown(KEY_W)) movementDelta.y += keyboardSpeed;  
        if (IsKeyDown(KEY_S)) movementDelta.y -= keyboardSpeed;  
        if (IsKeyDown(KEY_A)) movementDelta.x += keyboardSpeed;  
        if (IsKeyDown(KEY_D)) movementDelta.x -= keyboardSpeed;  
        
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

void InputHandler::HandleRoadPlacement() {
    if (!uiManager) {
        TraceLog(LOG_DEBUG, "No roadPlacement handler found");
        return;  // Safety check
    
    }

    if (IsKeyDown(KEY_B)) {
        uiManager->ModeSelect(UIMode::ROAD_BUILD);
    }

    if (IsKeyDown(KEY_X)) {
        uiManager->ModeSelect(UIMode::ROAD_DELETE);
    }

    if (IsKeyDown(KEY_ENTER)) {
        uiManager->ModeSelect(UIMode::IDLE);
    }
    
    Vector2 mousePos = GetMousePosition();
    Camera3D camera = cameraController->GetCamera();
    
    // Left mouse button for road placement
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Ray ray = GetScreenToWorldRay(mousePos, camera);
        Vector2 gridPos = uiManager->ScreenToGrid(ray);
        uiManager->StartSelection(gridPos);

        // Vector2 gridPos = roadPlacement->ScreenToGrid(mousePos, camera);
        // roadPlacement->StartPlacement(gridPos);
        
        TraceLog(LOG_INFO, "Mouse clicked at screen (%.0f, %.0f) -> grid (%.0f, %.0f)", 
                 mousePos.x, mousePos.y, gridPos.x, gridPos.y);
    }
    
    // Update placement while dragging
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // Vector2 gridPos = roadPlacement->ScreenToGrid(mousePos, camera);
        // roadPlacement->UpdatePlacement(gridPos);
        Ray ray = GetScreenToWorldRay(mousePos, camera);
        Vector2 gridPos = uiManager->ScreenToGrid(ray);
        uiManager->UpdateSelection(gridPos);
    }
    
    // Finish placement when mouse released
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        // roadPlacement->FinishPlacement();
        uiManager->EndSelection();
    }
    
    // Right mouse button to cancel placement
    // if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    //     if (roadPlacement->IsPlacing()) {
    //         roadPlacement->CancelPlacement();
    //     } else {
    //         Vector2 gridPos = roadPlacement->ScreenToGrid(mousePos, camera);
    //         roadPlacement->DeleteRoadPath(gridPos);
    //     }
        
    // }
}

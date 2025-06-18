#include "raylib.h"
#include "GameLoop.hpp"
#include "Game/Rendering/CameraController.hpp"

void GameLoop::Initialize(int screenW, int screenH) {
    InitWindow(screenW, screenH, "Games of Roads. Alpha 0.0.0");
    SetTargetFPS(60); 
    camera = new CameraController();
    camera->Initialize();
    grid = new GridRenderer();
    grid->Initialize();
    isRunning = 1;
}

void GameLoop::Render() {
    BeginDrawing();
    // ClearBackground(DARKGREEN);
    ClearBackground(SKYBLUE);

    BeginMode3D(camera->GetCamera());
        // Draw your 3D world here (grid, roads, etc.)
        grid->Render();
        DrawCube({0, 0, 0}, 2, 2, 2, RED);  // Test cube at origin
        DrawCube({50.0f, 0.0f, 50.0f}, 10.0f, 10.0f, 10.0f, RED);
        DrawCubeWires({50.0f, 0.0f, 50.0f}, 10.0f, 10.0f, 10.0f, WHITE);
        DrawCube({0.0f, 2.5f, 0.0f}, 5.0f, 5.0f, 5.0f, GREEN);
        DrawCube({100.0f, 2.5f, 100.0f}, 5.0f, 5.0f, 5.0f, BLUE);

        DrawLine3D({0, 0, 0}, {0, 20, 0}, PURPLE);  // Y-axis reference (up)
        
    EndMode3D();
    RenderDebugInfo();
    //DrawText("City Builder", 10, 10, 20, WHITE);
    DrawFPS(GetScreenWidth() - 80, 10);
    EndDrawing();
}

void GameLoop::RenderDebugInfo() {
    int yOffset = 160;  // Start below your existing UI text
    int lineHeight = 16;
    Color textColor = WHITE;
    Color labelColor = YELLOW;

    Camera3D cam = camera->GetCamera();
    
    // Camera Position
    DrawText("=== CAMERA DEBUG ===", 10, yOffset, 14, labelColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("Position: (%.2f, %.2f, %.2f)", 
             cam.position.x, cam.position.y, cam.position.z), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("Target:   (%.2f, %.2f, %.2f)", 
             cam.target.x, cam.target.y, cam.target.z), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("Up:       (%.2f, %.2f, %.2f)", 
             cam.up.x, cam.up.y, cam.up.z), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("FOV:      %.2f", cam.fovy), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("Projection: %s", 
             cam.projection == CAMERA_ORTHOGRAPHIC ? "ORTHOGRAPHIC" : "PERSPECTIVE"), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight + 5;
}


void GameLoop::Run() {
    while (!WindowShouldClose() && isRunning) {
        Update();
        Render();
    }
}

void GameLoop::Update() {
    camera->Update(); 
}

void GameLoop::Cleanup() {
    CloseWindow(); 
    delete camera;
    delete grid;
}
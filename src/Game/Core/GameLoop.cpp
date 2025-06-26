#include "raylib.h"
#include "GameLoop.hpp"
#include "Game/Rendering/CameraController.hpp"


void GameLoop::Initialize(int screenW, int screenH) {
    InitWindow(screenW, screenH, "Games of Roads. Alpha 0.0.0");
    SetTargetFPS(60); 

    worldHandler = new World();

    camera = new CameraController();
    camera->Initialize();
    grid = new GridRenderer();
    grid->Initialize();


    trafficNetwork = new TrafficNetwork(worldHandler);
    roadBuilder = new RoadBuilderService(worldHandler, trafficNetwork);

    uiManager = new UIManager(roadBuilder);

    input = new InputHandler();
    input->Initialize(camera, grid, uiManager);
    isRunning = 1;

    
}

void GameLoop::Render() {
    BeginDrawing();
    // ClearBackground(DARKGREEN);
    ClearBackground(SKYBLUE);

    BeginMode3D(camera->GetCamera());
        // Draw your 3D world here (grid, roads, etc.)
        // grid->Render();

        DrawCube({0, 0, 0}, 1, 1, 1, RED);  // Test cube at origin
        // DrawCube({50.0f, 0.0f, 50.0f}, 10.0f, 10.0f, 10.0f, RED);
        // DrawCubeWires({50.0f, 0.0f, 50.0f}, 10.0f, 10.0f, 10.0f, WHITE);
        // DrawCube({0.0f, 2.5f, 0.0f}, 5.0f, 5.0f, 5.0f, GREEN);
        // DrawCube({100.0f, 2.5f, 100.0f}, 5.0f, 5.0f, 5.0f, BLUE);

        DrawLine3D({0, 0, 0}, {0, 20, 0}, GREEN);  // Y-axis reference (up)
        DrawLine3D({0, 0, 0}, {30, 0, 0}, RED);  // X-axis reference (up)
        DrawLine3D({0, 0, 0}, {0, 0, 30}, BLUE);  // X-axis reference (up)
        uiManager->RenderSelection();
        RenderDebugInfo();    
    EndMode3D();
    
    uiManager->DrawTextInfo();
    DrawFPS(GetScreenWidth() - 80, 10);
    EndDrawing();
}

void GameLoop::RenderDebugInfo() {
    int yOffset = 160;  // Start below your existing UI text
    int lineHeight = 16;
    Color textColor = WHITE;
    Color labelColor = YELLOW;

    Camera3D cam = camera->GetCamera();
    trafficNetwork->DebugNodesIterator();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Ray ray = GetScreenToWorldRay(GetMousePosition(), cam);
        float t = -ray.position.y / ray.direction.y;
        Vector3 hitPoint = {
            ray.position.x + t * ray.direction.x,
            0.0f,
            ray.position.z + t * ray.direction.z
        };

        Vector3 chunk = worldHandler->GetChunkForPosition(hitPoint);
        BoundingBox box = worldHandler->GetChunkBorders(chunk);
        DrawBoundingBox(box, RED);
        grid->Render(box);
        // TraceLog(LOG_DEBUG, "Bounding box rendered for chunk (%f %f %f)", chunk.x, chunk.y, chunk.z);
    }

    
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
    
}


void GameLoop::Run() {
    while (!WindowShouldClose() && isRunning) {
        Update();
        Render();
    }
}

void GameLoop::Update() {
    // camera->Update(); 
    input->ProcessInput();
}

void GameLoop::Cleanup() {
    CloseWindow(); 
    delete worldHandler;
    delete trafficNetwork;
    delete roadBuilder;
    delete uiManager;
    delete input;
    delete camera;
    delete grid;
}
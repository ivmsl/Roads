#include "raylib.h"
#include "raymath.h"
#include "UIManager.hpp"
#include <functional>
#include "Game/Core/Helpers.hpp"


UIManager::UIManager(RoadBuilderService* rb) {

    isSelecting = false;
    startGridPos = {-1, -1};
    currentGridPos = {-1, -1};
    roadBuilder = rb;
}

Vector2 UIManager::ScreenToWorld(Ray ray) {

    // Find where ray hits the ground plane (y = 0)
    float t = -ray.position.y / ray.direction.y;
    Vector3 hitPoint = {
        ray.position.x + t * ray.direction.x,
        0.0f,
        ray.position.z + t * ray.direction.z
    };
    
    return {(float)hitPoint.x, (float)hitPoint.z};

}

void UIManager::StartSelection(Vector2 gridPos) {

    isSelecting = true;
    startGridPos = gridPos;
    currentGridPos = gridPos;
    
    TraceLog(LOG_DEBUG, "Selected grid at (%d, %d)", 
             (int)gridPos.x, (int)gridPos.y);
}

void UIManager::UpdateSelection(Vector2 gridPos) {
    if (!isSelecting) return;   
    currentGridPos = gridPos;

    // TraceLog(LOG_DEBUG, "\n\nUI Manager: tracing osbtruction;");
        Vector3 startPos = {startGridPos.x, 0.0f, startGridPos.y};
        Vector3 endPos = {currentGridPos.x, 0.0f, currentGridPos.y};

       
        if (roadBuilder->CheckIfObstructedOnTheLine(startPos, endPos)) {
            brickColor = RED;
        }

        RoadBuilderService::NodeHeadTailInfo conn = roadBuilder->CheckNodesStartFinish(startPos, endPos);
        switch (conn)
        {
        case RoadBuilderService::NodeHeadTailInfo::NODE_START: {
            brickColor = YELLOW;
            break;
        }
        case RoadBuilderService::NodeHeadTailInfo::NODE_END: {
            brickColor = GREEN;
            break;
        }
        case RoadBuilderService::NodeHeadTailInfo::NODE_STARTEND:  {
            brickColor = MAGENTA;
            break;
        }
        default:
            brickColor = BLUE;
            break;
        }
}

void UIManager::EndSelection() {
    if (!isSelecting) return;
    isSelecting = false;
    CompleteSelectionAction();
}

void UIManager::CompleteSelectionAction() {

    brickColor = WHITE;
    if (mode == UIMode::IDLE) {
        
    }

    if (mode == UIMode::MAKE_NODES) {
        

        TraceLog(LOG_DEBUG, "Node making process initialised");
        Vector3 startPos = {startGridPos.x, 0.0f, startGridPos.y};
        Vector3 endPos = {currentGridPos.x, 0.0f, currentGridPos.y};

        if (!roadBuilder->CheckIfObstructedOnTheLine(startPos, endPos)) {
            roadBuilder->BuildRoad(startPos, endPos);
        } else {
            TraceLog(LOG_DEBUG, "Can not build! Error: path is abstructed");
        }
    }
}

void UIManager::CancelSelection() {
    if (!isSelecting) return;
    isSelecting = false;

    TraceLog(LOG_DEBUG, "Cancelled road placement");
}

void UIManager::RenderSelection() {
    if (!isSelecting) return;

    float dx = currentGridPos.x - startGridPos.x;
    float dz = currentGridPos.y - startGridPos.y;
    
    // Calculate total distance
    float totalDistance = std::sqrt(dx * dx + dz * dz);

    int numSteps = (int)(totalDistance / 10); //byl +1 ale koniec potem dodalismy

     for (int i = 0; i < numSteps; i++) {
        float t = (numSteps > 1) ? (float)i / (numSteps - 1) : 0.0f; // t goes from 0 to 1
        
        
        float x = startGridPos.x + dx * t;
        float y = startGridPos.y + dz * t;

        RenderSelectionBrick(x, y);
     }
}

void UIManager::RenderSelectionBrick(int x, int y) {
    // Convert grid to world coordinates
    // float worldX = x * 10.0f + 5.0f;  // Center of tile
    // float worldZ = y * 10.0f + 5.0f;
    // float worldY = 2.0f;  // Slightly above regular roads
    
    Vector3 position = {(float)x, 0.0f, (float)y};
    Vector3 size;
    Color color;
    
    
    size = {10.0f, 1.0f, 10.0f};
    color = {255, 255, 255, 100}; 
    
    DrawCube(position, size.x, size.y, size.z, brickColor);
    DrawCubeWires(position, size.x, size.y, size.z, BLUE);
}

void UIManager::DrawTextInfo() {
    int yOffset = 160;  
    int lineHeight = 16;
    Color textColor = WHITE;
    Color labelColor = YELLOW;

    DrawText("=== SELECTION INFO ===", 10, yOffset, 14, labelColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("Starting position: (%.2f, %.2f)", 
             startGridPos.x, startGridPos.y), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("End position:   (%.2f, %.2f)", 
             currentGridPos.x, currentGridPos.y), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;



    float dx = startGridPos.x - currentGridPos.x;
    float dz = startGridPos.y - currentGridPos.y;
    
    // Calculate total distance
    float totalDistance = std::sqrt(dx * dx + dz * dz);
    int numSteps = (int)(totalDistance / 10.0f); //byl +1 ale koniec potem dodalismy


    DrawText(TextFormat("Len in blocks:   (%i, %.2f)", 
             numSteps, totalDistance), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
}

void UIManager::ModeSelect(UIMode newMode) {
    mode = newMode;
}
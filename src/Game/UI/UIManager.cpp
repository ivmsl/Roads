#include "raylib.h"
#include "raymath.h"
#include "Game/Roads/RoadManager.hpp"
#include "Game/Roads/RoadAlgorythms.hpp"
#include "UIManager.hpp"
#include <functional>


UIManager::UIManager(RoadManager* rm) {
    isSelecting = false;
    Vector2 startGridPos = {-1, -1};
    Vector2 currentGridPos = {-1, -1};
    roadManager = rm;
    metadata = {0, RoadDirection::HORIZONTAL};

    SetAlgorytm(RoadAlgorythms::LinePlacementAlgorythm);
}

void UIManager::SetAlgorytm(TraceAlgorytm alg) {
    selectedAlgorytm = alg;
}

Vector2 UIManager::ScreenToGrid(Ray ray) {

    // Find where ray hits the ground plane (y = 0)
    float t = -ray.position.y / ray.direction.y;
    Vector3 hitPoint = {
        ray.position.x + t * ray.direction.x,
        0.0f,
        ray.position.z + t * ray.direction.z
    };

    // Convert world coordinates to grid coordinates
    // Each tile is 10x10 meters, so divide by 10 and floor
    int gridX = (int)floor(hitPoint.x / 10.0f);
    int gridY = (int)floor(hitPoint.z / 10.0f);
    
    // Clamp to grid bounds (0-99)
    gridX = Clamp(gridX, 0, 99);
    gridY = Clamp(gridY, 0, 99);
    
    return {(float)gridX, (float)gridY};

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
}

void UIManager::EndSelection() {
    if (!isSelecting) return;
    isSelecting = false;
    CompleteSelectionAction();
}

void UIManager::CompleteSelectionAction() {
    if (mode == UIMode::ROAD_BUILD) {

        metadata.direction = DetermineRoadDirection();
        if (metadata.direction == RoadDirection::HORIZONTAL || metadata.direction == RoadDirection::HORISONTAL_MIN) {
            metadata.length = std::abs(startGridPos.x - currentGridPos.x);
        } else {
            metadata.length = std::abs(startGridPos.y - currentGridPos.y);
        }

        //  RoadDirection previewDirection = DetermineRoadDirection();
         selectedAlgorytm(startGridPos, currentGridPos, metadata, [this](int x, int y, SelectionMetadata mtd) {
                    roadManager->PlaceRoad(x, y, mtd); }
                    );
    }
    if (mode == UIMode::ROAD_DELETE) {
        // RoadDirection previewDirection = DetermineRoadDirection();
        metadata.direction = DetermineRoadDirection();
        selectedAlgorytm(startGridPos, currentGridPos, metadata, [this](int x, int y, SelectionMetadata mtd) {
                    roadManager->DeleteRoad(x, y, mtd); }
                    );
    }
}

void UIManager::CancelSelection() {
    if (!isSelecting) return;
    isSelecting = false;

    TraceLog(LOG_DEBUG, "Cancelled road placement");
}

void UIManager::RenderSelection() {
    if (!isSelecting) return;
    
    metadata.direction = DetermineRoadDirection();
    //metadata.length
    // Determine preview direction
    // RoadDirection previewDirection = DetermineRoadDirection();
    
    // Render preview path
    selectedAlgorytm(startGridPos, currentGridPos, metadata, [this](int x, int y, SelectionMetadata mtd) {
                    RenderSelectionBrick(x, y, mtd); }
                    );
}

void UIManager::RenderSelectionBrick(int x, int y, SelectionMetadata mtd) {
    // Convert grid to world coordinates
    float worldX = x * 10.0f + 5.0f;  // Center of tile
    float worldZ = y * 10.0f + 5.0f;
    float worldY = 2.0f;  // Slightly above regular roads
    
    Vector3 position = {worldX, worldY, worldZ};
    Vector3 size;
    Color color;
    
    // Ghost/preview styling - semi-transparent
    if (mtd.direction == RoadDirection::HORIZONTAL || mtd.direction == RoadDirection::HORISONTAL_MIN) {
        size = {10.0f, 1.0f, 8.0f};
        color = {255, 255, 255, 100}; 
    } else {
        size = {8.0f, 1.0f, 10.0f};
        color = {255, 255, 255, 100};  
    }
    
    DrawCube(position, size.x, size.y, size.z, color);
    DrawCubeWires(position, size.x, size.y, size.z, WHITE);
}


inline RoadDirection UIManager::DetermineRoadDirection() {
    int deltaX = (int)currentGridPos.x - (int)startGridPos.x;
    int deltaY = (int)currentGridPos.y - (int)startGridPos.y;
    
    if (abs(deltaX) >= abs(deltaY)) {
        if (deltaX < 0) {
            return RoadDirection::HORISONTAL_MIN;
        } 
        return RoadDirection::HORIZONTAL;
    } else {
        if (deltaY < 0) {
            return RoadDirection::VERTICAL_MIN;
        }
        return RoadDirection::VERTICAL;
    }
}

void UIManager::ModeSelect(UIMode newMode) {
    mode = newMode;
}
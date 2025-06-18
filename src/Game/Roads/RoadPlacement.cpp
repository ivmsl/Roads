#include "raylib.h"
#include "raymath.h"
#include "RoadPlacement.hpp"

Vector2 RoadPlacement::ScreenToGrid(Vector2 screenPos, Camera3D camera) {

    Ray ray = GetMouseRay(screenPos, camera);

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

void RoadPlacement::StartPlacement(Vector2 gridPos) {
    if (!roadManager->IsValidPosition((int)gridPos.x, (int)gridPos.y)) {
        return;  // Can't place here
    }
    
    isPlacing = true;
    startGridPos = gridPos;
    currentGridPos = gridPos;
    
    // Debug output
    TraceLog(LOG_INFO, "Started road placement at grid (%d, %d)", 
             (int)gridPos.x, (int)gridPos.y);
}

void RoadPlacement::UpdatePlacement(Vector2 gridPos) {
    if (!isPlacing) return;
    
    currentGridPos = gridPos;
    
    // Optional: Debug output for drag updates
    // TraceLog(LOG_DEBUG, "Dragging to grid (%d, %d)", (int)gridPos.x, (int)gridPos.y);
}

void RoadPlacement::FinishPlacement() {
    if (!isPlacing) return;
    
    // Determine road direction based on drag
    RoadDirection direction = DetermineRoadDirection();
    
    // Place roads along the path
    PlaceRoadPath(direction);
    
    isPlacing = false;
    
    TraceLog(LOG_INFO, "Finished road placement from (%d, %d) to (%d, %d)", 
             (int)startGridPos.x, (int)startGridPos.y,
             (int)currentGridPos.x, (int)currentGridPos.y);
}

void RoadPlacement::CancelPlacement() {
    if (!isPlacing) return;
    
    isPlacing = false;
    TraceLog(LOG_INFO, "Cancelled road placement");
}

inline RoadDirection RoadPlacement::DetermineRoadDirection() {
    int deltaX = (int)currentGridPos.x - (int)startGridPos.x;
    int deltaY = (int)currentGridPos.y - (int)startGridPos.y;
    
    // If drag is more horizontal than vertical, place horizontal roads
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

void RoadPlacement::PlaceRoadPath(RoadDirection direction) {
    // For now, just place a single road at start position
    // Later we can extend this to place roads along the entire drag path
    
    if (direction == RoadDirection::HORIZONTAL || direction == RoadDirection::HORISONTAL_MIN) {
        // Place horizontal roads from start to current X position
        int startX = std::min((int)startGridPos.x, (int)currentGridPos.x);
        int endX = std::max((int)startGridPos.x, (int)currentGridPos.x);
        int y = (int)startGridPos.y;
        
        for (int x = startX; x <= endX; x++) {
            if (roadManager->IsValidPosition(x, y)) {
                roadManager->PlaceRoad(x, y, RoadDirection::HORIZONTAL);
            }
        }
    } else {
        // Place vertical roads from start to current Y position
        int startY = std::min((int)startGridPos.y, (int)currentGridPos.y);
        int endY = std::max((int)startGridPos.y, (int)currentGridPos.y);
        int x = (int)startGridPos.x;
        
        for (int y = startY; y <= endY; y++) {
            if (roadManager->IsValidPosition(x, y)) {
                roadManager->PlaceRoad(x, y, RoadDirection::VERTICAL);
            }
        }
    }
}

void RoadPlacement::RenderPreview() {
    if (!isPlacing) return;
    
    // Determine preview direction
    RoadDirection previewDirection = DetermineRoadDirection();
    
    // Render preview path
    if (previewDirection == RoadDirection::HORIZONTAL || previewDirection == RoadDirection::HORISONTAL_MIN) {
        int startX = std::min((int)startGridPos.x, (int)currentGridPos.x);
        int endX = std::max((int)startGridPos.x, (int)currentGridPos.x);
        int y = (int)startGridPos.y;
        
        for (int x = startX; x <= endX; x++) {
            RenderPreviewTile(x, y, RoadDirection::HORIZONTAL);
        }
    } else {
        int startY = std::min((int)startGridPos.y, (int)currentGridPos.y);
        int endY = std::max((int)startGridPos.y, (int)currentGridPos.y);
        int x = (int)startGridPos.x;
        
        for (int y = startY; y <= endY; y++) {
            RenderPreviewTile(x, y, RoadDirection::VERTICAL);
        }
    }
}

void RoadPlacement::RenderPreviewTile(int x, int y, RoadDirection direction) {
    // Convert grid to world coordinates
    float worldX = x * 10.0f + 5.0f;  // Center of tile
    float worldZ = y * 10.0f + 5.0f;
    float worldY = 2.0f;  // Slightly above regular roads
    
    Vector3 position = {worldX, worldY, worldZ};
    Vector3 size;
    Color color;
    
    // Ghost/preview styling - semi-transparent
    if (direction == RoadDirection::HORIZONTAL) {
        size = {10.0f, 1.0f, 8.0f};
        color = {255, 255, 255, 100};  // Semi-transparent white
    } else {
        size = {8.0f, 1.0f, 10.0f};
        color = {255, 255, 255, 100};  // Semi-transparent white
    }
    
    DrawCube(position, size.x, size.y, size.z, color);
    DrawCubeWires(position, size.x, size.y, size.z, WHITE);
}

void RoadPlacement::DeleteRoadPath(Vector2 gridPos) {
    int x = (int)gridPos.x; int y = (int)gridPos.y;
    if (roadManager->IsValidPosition(x, y)) {
        roadManager->DeleteRoad(x, y);
    }

}

bool RoadPlacement::IsPlacing() {
    return isPlacing;
}
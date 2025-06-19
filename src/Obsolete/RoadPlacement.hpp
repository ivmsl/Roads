#ifndef ROAD_PLACEMENT_HPP_
#define ROAD_PLACEMENT_HPP_

#include "raylib.h"
#include "RoadManager.hpp"

class RoadPlacement {
private:
    RoadManager* roadManager;
    bool isPlacing = false;
    Vector2 startGridPos;
    Vector2 currentGridPos;
    
public:
    RoadPlacement(RoadManager* manager) : roadManager(manager) {}
    bool IsPlacing();
    Vector2 ScreenToGrid(Vector2 screenPos, Camera3D camera);
    void StartPlacement(Vector2 gridPos);
    void UpdatePlacement(Vector2 gridPos);
    void PlaceRoadPath(RoadDirection direction);
    void DeleteRoadPath(Vector2 gridPos);
    void FinishPlacement();
    void CancelPlacement();
    void RenderPreview();  
    void RenderPreviewTile(int x, int y, RoadDirection direction);
    inline RoadDirection DetermineRoadDirection();
};

#endif
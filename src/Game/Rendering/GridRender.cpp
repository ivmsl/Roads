#include "raylib.h"
#include "GridRender.hpp"

void GridRenderer::Initialize() {
    gridVisible = true;
}

void GridRenderer::Render() {
    if (!gridVisible) return;
    float worldSize = gridSize * tileSize;

    for (int i = 0; i <= gridSize; i++) {
        float x = i * tileSize;
        
        // Choose color based on major/minor line
        Color lineColor = (i % majorLineInterval == 0) ? majorLineColor : minorLineColor;
        
        Vector3 start = {x, 0.0f, 0.0f};
        Vector3 end = {x, 0.0f, worldSize};
        
        DrawLine3D(start, end, lineColor);
    }

    for (int i = 0; i <= gridSize; i++) {
        float z = i * tileSize;
        
        // Choose color based on major/minor line
        Color lineColor = (i % majorLineInterval == 0) ? majorLineColor : minorLineColor;
        
        Vector3 start = {0.0f, 0.0f, z};
        Vector3 end = {worldSize, 0.0f, z};
        
        DrawLine3D(start, end, lineColor);
    }
}

void GridRenderer::ToggleVisibility() {
    gridVisible = !gridVisible;
}
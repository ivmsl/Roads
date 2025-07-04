#include "raylib.h"
#include "GridRender.hpp"




void GridRenderer::Initialize() {
    gridVisible = true;
    mode = GridMode::TOTAL;
}

void GridRenderer::Render() {
    if (!gridVisible) return;
    if (mode == GridMode::TOTAL) { 
        float worldSize = gridSize * tileSize;

        for (int i = 0; i <= gridSize; i++) {
            float x = i * tileSize;

        
            Color lineColor = (i % majorLineInterval == 0) ? majorLineColor : minorLineColor;
        
            Vector3 start = {x, 0.0f, 0.0f};
            Vector3 end = {x, 0.0f, worldSize};
        
            DrawLine3D(start, end, lineColor);
        }

        for (int i = 0; i <= gridSize; i++) {
            float z = i * tileSize;
        
    
        Color lineColor = (i % majorLineInterval == 0) ? majorLineColor : minorLineColor;       
        Vector3 start = {0.0f, 0.0f, z};
        Vector3 end = {worldSize, 0.0f, z};
        
        DrawLine3D(start, end, lineColor);
    }

    }

    
}

void GridRenderer::ToggleVisibility() {
    gridVisible = !gridVisible;
}

void GridRenderer::Render(BoundingBox chunkBounds) {

    // TraceLog(LOG_DEBUG, "Render Grid: from %f to %f", chunkBounds.min.z, chunkBounds.max.z);
    for (int i = chunkBounds.min.x / tileSize; i <= chunkBounds.max.x / tileSize; i++) {
            float x = i * tileSize;
            Color lineColor = (i % majorLineInterval == 0) ? majorLineColor : minorLineColor;
        
            Vector3 start = {x, 0.0f, chunkBounds.min.z};
            Vector3 end = {x, 0.0f, chunkBounds.max.z};
        
            DrawLine3D(start, end, lineColor);        
    }

    for (int i = chunkBounds.min.z / tileSize; i <= chunkBounds.max.z / tileSize; i++) {
        float z = i * tileSize;

        Color lineColor = (i % majorLineInterval == 0) ? majorLineColor : minorLineColor;
        
        Vector3 start = {chunkBounds.min.x, 0.0f, z};
        Vector3 end = {chunkBounds.max.x, 0.0f, z};
        
        DrawLine3D(start, end, lineColor);        
    }
}
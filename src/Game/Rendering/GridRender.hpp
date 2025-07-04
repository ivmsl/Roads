#ifndef GRID_RENDER_HPP_
#define GRID_RENDER_HPP_

#include "raylib.h"

enum GridMode {
    TOTAL,
    CHUNK
};

class GridRenderer {
    private:
        bool gridVisible = true;
        int gridSize = 100;        // 100x100 blokow
        float tileSize = 10.0f;    
        Color majorLineColor = GRAY;
        Color minorLineColor = LIGHTGRAY;
        int majorLineInterval = 10; 

        GridMode mode;
    
    public:
        void Initialize();
        void Render();
        void Render(BoundingBox chunkBounds);
        void ToggleVisibility();
        bool IsVisible() { return gridVisible; }
};

#endif

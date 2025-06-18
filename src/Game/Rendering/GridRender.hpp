#ifndef GRID_RENDER_HPP_
#define GRID_RENDER_HPP_

#include "raylib.h"

class GridRenderer {
    private:
        bool gridVisible = true;
        int gridSize = 100;        // 100x100 tiles
        float tileSize = 10.0f;    // 10x10 meters per tile
        Color majorLineColor = GRAY;
        Color minorLineColor = LIGHTGRAY;
        int majorLineInterval = 10; // Every 10th line is major
    
    public:
        void Initialize();
        void Render();
        void ToggleVisibility();
        bool IsVisible() { return gridVisible; }
};

#endif

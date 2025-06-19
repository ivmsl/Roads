#include "RoadTile.hpp"
#include "RoadManager.hpp"
#include "raylib.h"

void RoadTile::Render() const {
    float worldX = x * 10.0f;
    float worldZ = y * 10.0f;
    float worldY = 1.0f;  // Slightly above ground level

    Vector3 position = {worldX + 5.0f, worldY, worldZ + 5.0f};  // Center of tile
    Vector3 size;
    Color color;

    if (direction == RoadDirection::HORIZONTAL) {
        size = {10.0f, 2.0f, 8.0f};  // Full width, narrow height for road feel
        color = GRAY;
    } else { // VERTICAL
        size = {8.0f, 2.0f, 10.0f};  // Narrow width, full height
        color = DARKGRAY;
    }
    
    DrawCube(position, size.x, size.y, size.z, color);
    DrawCubeWires(position, size.x, size.y, size.z, BLACK);  // Outline for clarity

}

RoadDirection RoadTile::GetDirection() {
    return direction;
}


void GravelRoad::Render() const {
        float worldX = x * 10.0f;
        float worldZ = y * 10.0f;
        float worldY = 0.5f;  // Slightly lower than regular roads
        
        // Road dimensions
        Vector3 position = {worldX + 5.0f, worldY, worldZ + 5.0f};  // Center of tile
        Vector3 size;
        Color color;
        
        // Gravel road styling - brown/tan colors
        if (direction == RoadDirection::HORIZONTAL || direction == RoadDirection::HORISONTAL_MIN) {
            size = {10.0f, 1.5f, 8.0f};  // Slightly smaller than regular roads
            color = BROWN;
        } else { // VERTICAL
            size = {8.0f, 1.5f, 10.0f};
            color = {139, 69, 19, 255};  // Darker brown (saddle brown)
        }
        
        DrawCube(position, size.x, size.y, size.z, color);
        DrawCubeWires(position, size.x, size.y, size.z, DARKBROWN);  // Brown outline
    
}

void IntersectionTile::Render() const {
        float worldX = x * 10.0f;
        float worldZ = y * 10.0f;
        float worldY = 0.5f; 
        
        // Road dimensions
        Vector3 position = {worldX + 5.0f, worldY, worldZ + 5.0f};  // Center of tile
        Vector3 size = {8.0f, 1.5f, 8.0f};
        Color color = RED;
        
        DrawCube(position, size.x, size.y, size.z, color);
        DrawCubeWires(position, size.x, size.y, size.z, WHITE);  

        if (connectionFlags & IntersectionConnection::NORTH) {
            Vector3 smallBlock = {position.x, position.y, position.z + 4.5f};
            Vector3 sizeSmall = {8.0f, 1.5, 0.8f};
            DrawCube(smallBlock, sizeSmall.x, sizeSmall.y, sizeSmall.z, GREEN);
        }

        if (connectionFlags & IntersectionConnection::SOUTH) {
            Vector3 smallBlock = {position.x, position.y, position.z - 4.5f};
            Vector3 sizeSmall = {8.0f, 1.5, 0.8f};
            DrawCube(smallBlock, sizeSmall.x, sizeSmall.y, sizeSmall.z, YELLOW);
        }
        if (connectionFlags & IntersectionConnection::EAST) {
            Vector3 smallBlock = {position.x - 4.5f, position.y, position.z};
            Vector3 sizeSmall = {0.8f, 1.5, 8.0f};
            DrawCube(smallBlock, sizeSmall.x, sizeSmall.y, sizeSmall.z, MAGENTA);
        }
        if (connectionFlags & IntersectionConnection::WEST) {

            Vector3 smallBlock = {position.x + 4.5f, position.y, position.z};
            Vector3 sizeSmall = {0.8f, 1.5, 8.0f};
            DrawCube(smallBlock, sizeSmall.x, sizeSmall.y, sizeSmall.z, PINK);
        }




}

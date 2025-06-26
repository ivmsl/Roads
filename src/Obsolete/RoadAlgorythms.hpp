#include <functional>
#include "Tile.hpp"
#include "raylib.h"


class RoadAlgorythms {
    public:
        //static so it's avaliable without instantiating RA
       static void LinePlacementAlgorythm(Vector2 startGridPos, Vector2 currentGridPos, RoadPlacementMetadata mtd, std::function<void(int x, int y, RoadPlacementMetadata mtd)> action) {
            RoadDirection direction = mtd.direction;
            
            if (direction == RoadDirection::HORIZONTAL || direction == RoadDirection::HORISONTAL_MIN) {
                // Place horizontal roads from start to current X position
                int startX = std::min((int)startGridPos.x, (int)currentGridPos.x);
                int endX = std::max((int)startGridPos.x, (int)currentGridPos.x);
                int y = (int)startGridPos.y;
                
                for (int x = startX; x <= endX; x+=10) {
                        action(x, y, mtd);
                        mtd.length--;
                }
            } else {
                // Place vertical roads from start to current Y position
                int startY = std::min((int)startGridPos.y, (int)currentGridPos.y);
                int endY = std::max((int)startGridPos.y, (int)currentGridPos.y);
                int x = (int)startGridPos.x;
                
                for (int y = startY; y <= endY; y+=10) {
                    action(x, y, mtd);
                    mtd.length--;
                }
            }
    }


    static void AbsoluteLine(Vector2 startGridPos, Vector2 currentGridPos, RoadPlacementMetadata mtd, std::function<void(int x, int y, RoadPlacementMetadata mtd)> action) {
            // RoadDirection direction = mtd.direction;
            
            float dx = currentGridPos.x - startGridPos.x;
            float dy = currentGridPos.y - startGridPos.y;
            float totalDistance = std::sqrt(dx * dx + dy * dy);

     if (totalDistance < 1.0f) {
        action((int)startGridPos.x, (int)startGridPos.y, mtd);
        return;
    }
    
    // Normalize direction vector
    float dirX = dx / totalDistance;
    float dirY = dy / totalDistance;
    
    // Place blocks every 10 units along the line
    for (float distance = 0.0f; distance <= totalDistance; distance += 10.0f) {
        Vector2 blockPos = {
            startGridPos.x + dirX * distance,
            startGridPos.y + dirY * distance
        };
        
        action((int)blockPos.x, (int)blockPos.y, mtd);
    }
    
    // Always place a block at the end position
    action((int)currentGridPos.x, (int)currentGridPos.y, mtd);
    }
};

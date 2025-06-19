#include <functional>
#include "Tile.hpp"
#include "raylib.h"


class RoadAlgorythms {
    public:
        //static so it's avaliable without instantiating RA
       static void LinePlacementAlgorythm(Vector2 startGridPos, Vector2 currentGridPos, RoadDirection direction, std::function<void(int x, int y, RoadDirection dir)> action) {
            if (direction == RoadDirection::HORIZONTAL || direction == RoadDirection::HORISONTAL_MIN) {
                // Place horizontal roads from start to current X position
                int startX = std::min((int)startGridPos.x, (int)currentGridPos.x);
                int endX = std::max((int)startGridPos.x, (int)currentGridPos.x);
                int y = (int)startGridPos.y;
                
                for (int x = startX; x <= endX; x++) {
                        action(x, y, RoadDirection::HORIZONTAL);
                }
            } else {
                // Place vertical roads from start to current Y position
                int startY = std::min((int)startGridPos.y, (int)currentGridPos.y);
                int endY = std::max((int)startGridPos.y, (int)currentGridPos.y);
                int x = (int)startGridPos.x;
                
                for (int y = startY; y <= endY; y++) {
                    action(x, y, RoadDirection::VERTICAL);
                }
            }
    }
};


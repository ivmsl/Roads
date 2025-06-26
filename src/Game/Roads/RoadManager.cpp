#include "raylib.h"
#include "RoadManager.hpp"

RoadManager::RoadManager() {
        xSize = 100, ySize = 100; //GRID SIZE REPLACE LATER

        for(int x = 0; x < xSize; x++) {
            for(int y = 0; y < ySize; y++) {
                grid[x][y] = std::make_unique<EmptyTile>(x, y); //zamiast new — robimy make_unique
            }
        }
    }

void RoadManager::PlaceRoad(int x, int y, RoadPlacementMetadata mtd) {
        RoadDirection dir = mtd.direction; 
        if (RoadTile* road = GetRoad(x, y)) {
            if (dir != road->GetDirection()) {
                int connF = IntersectionConnection::START;
                if (GetRoad(x + 1, y)) {
                    connF |= IntersectionConnection::WEST;
                    TraceLog(LOG_DEBUG, "Got a road on X + 1"); //!!!!
                }
                if (GetRoad(x - 1, y)) {
                    connF |= IntersectionConnection::EAST;
                    TraceLog(LOG_DEBUG, "Got a road on X - 1");
                }
                if (GetRoad(x, y + 1)) {
                    connF |= IntersectionConnection::NORTH;
                    TraceLog(LOG_DEBUG, "Got a road on Y + 1"); ////!!!!!!!!
                }
                if (GetRoad(x, y - 1)) {
                    TraceLog(LOG_DEBUG, "Got a road on Y - 1");
                    connF |= IntersectionConnection::SOUTH;
                }

                if (connF != 0) {
                    if ((dir == RoadDirection::HORIZONTAL && mtd.length > 0) || (dir == RoadDirection::HORISONTAL_MIN && mtd.length > 0)) {
                        connF |= IntersectionConnection::WEST;
                    }
                    if ((dir == RoadDirection::VERTICAL && mtd.length > 0) || (dir == RoadDirection::VERTICAL_MIN && mtd.length > 0)) {
                         connF |= IntersectionConnection::NORTH;
                    }
                }


                TraceLog(LOG_DEBUG, "connF is %d", connF);

                TileToIntersection(x, y, connF);
                return;
            }
        }

        if (grid[x][y]->CanPlaceRoad()) {
            grid[x][y] = std::make_unique<GravelRoad>(x, y, dir);  //TODO: Zamienic na inne rodzaje drog
        }
    }

void RoadManager::DeleteRoad(int x, int y, RoadPlacementMetadata mtd) {
    if (GetRoad(x, y)) {
        grid[x][y] = std::make_unique<EmptyTile>(x, y);
    }
}

RoadTile* RoadManager::GetRoad(int x, int y) {
    if (!IsValidPosition(x, y)) {return nullptr;}
    return grid[x][y]->As<RoadTile>();  // Kastujemy wszystko jako road tile
}

bool RoadManager::IsValidPosition(int x, int y) {
    return (x >= xSize || y >= xSize || x < 0 || y < 0) ? false : true; 
}


//TODO!: Wymyslic bardziej sprawny rendering
void RoadManager::Render() {
    for(int x = 0; x < xSize; x++) {
        for(int y = 0; y < ySize; y++) {
            grid[x][y]->Render();
        }
    }
}

void RoadManager::TileToIntersection(int x, int y, int connType) {
    if (IsValidPosition(x, y)) {
        grid[x][y] = std::make_unique<IntersectionTile>(x, y, connType);
    }
}
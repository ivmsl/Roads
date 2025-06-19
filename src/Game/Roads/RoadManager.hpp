#ifndef ROAD_MANAGER_HPP_
#define ROAD_MANAGER_HPP_

#include <memory>
#include "Tile.hpp"
#include "RoadTile.hpp"
#include "Game/UI/Helpers.hpp"


using RoadPlacementMetadata = SelectionMetadata;

class RoadManager {
    private:
        std::unique_ptr<Tile> grid[100][100];  // "Smart pointers" — automatyczna dealokacja objektow po wyjsciu poza zasierg
        int xSize, ySize = 0;

    public:
        
        RoadManager();
        void PlaceRoad(int x, int y, RoadPlacementMetadata mtd);
        void DeleteRoad(int x, int y, RoadPlacementMetadata mtd);
        void TileToIntersection(int x, int y, int connType);
        RoadTile* GetRoad(int x, int y);
        bool IsValidPosition(int x, int y);
        void Render();
};  

#endif 
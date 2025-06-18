#ifndef ROAD_TILE_HPP_ 
#define ROAD_TILE_HPP_

#include "Tile.hpp"
#include <vector>

class RoadTile : public Tile {
    protected:
        // std::vector<Lane> lanes;
        RoadDirection direction;
        float speedLimit;
        
    public:
        RoadTile(int x, int y, RoadDirection dir) 
            : Tile(x, y, TileType::ROAD), direction(dir), speedLimit(50.0f) {}
        RoadDirection GetDirection();
        // virtual void AddLane(Lane lane) { lanes.push_back(lane); }
        // const std::vector<Lane>& GetLanes() const { return lanes; }
        // int GetLaneCount() const { return lanes.size(); }
        
        void Render() const override;  // Base road rendering
};

class IntersectionTile : public RoadTile {
    protected:
        // std::vector<Lane> lanes;
        int connectionFlags; //with IntersectionConnection !!!
        RoadDirection direction;
        
    public:
        IntersectionTile(int x, int y, int connF) : RoadTile(x, y, RoadDirection::HORIZONTAL), connectionFlags(connF) {
            type = TileType::INTERSECTION;
        }
        
        void Render() const override;  
};

class GravelRoad : public RoadTile {
    public:
        GravelRoad(int x, int y, RoadDirection dir) : RoadTile(x, y, dir) {
            speedLimit = 30.0f;
        // Add default lanes for one-way two-lane gravel road
        // lanes.push_back(Lane{dir, 0});  // Lane 0
        // lanes.push_back(Lane{dir, 1});  // Lane 1
        }
        void Render() const override; 
};

#endif
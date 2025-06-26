#ifndef WORLD_HPP_
#define WORLD_HPP_

#include "raylib.h"
#include <vector>
#include "Game/Traffic/TrafficNode.hpp"
#include <unordered_map>
#include "Game/Core/Helpers.hpp"

class Chunk {

};

class TrafficNode;
class RoadSegment;

class World {
    private:
        float chunkSize = 10.0f * 10.0f;
        std::unordered_map<Vector3, std::vector<TrafficNode*>> nodes;
        std::unordered_map<Vector3, std::vector<RoadSegment*>> roads;
        // (chunk): [node1, node2, node3];
    public:
        Vector3 GetChunkForPosition(Vector3 worldPosition);
        BoundingBox GetChunkBorders(Vector3 chunk);
        
        TrafficNode* RegisterTrafficNode(TrafficNode* node);
        TrafficNode* DeregisterTrafficNode(TrafficNode* node);

        TrafficNode* FindNearestNode(Vector3 worldPosition, float maxRadius = 8.0f);

        RoadSegment* RegisterRoad(RoadSegment* road);
        RoadSegment* DeregisterRoad(RoadSegment* road);

        RoadSegment* FindNearestRoad(Vector3 worldPosition, float maxRadius = 8.0f);

        std::vector<Vector3> GetChunksCrossedBySegment(Vector3 start, Vector3 end);

};      

#endif
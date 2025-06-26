#ifndef TRAFFIC_NODE_HPP_
#define TRAFFIC_NODE_HPP_

#include <vector>
#include "raylib.h"
#include "Game/Debug/DebugRenderer.hpp"
#include "Game/World/World.hpp"
#include "Game/Core/Helpers.hpp"
#include <memory>

class World;

class TrafficNode : IDebuggable {
    private: 
        Vector3 worldPosition;     // 3D position in world space
        Vector3 chunkInfo;
        std::vector<TrafficNode*> connections;  // Adjacent nodes
        float speedLimit;
    
    public: 
        TrafficNode(Vector3 position) : worldPosition(position) {};
        
        void SetChunkInfo(Vector3 chunk);
        Vector3 GetChunkInfo() const { return chunkInfo; };

        Vector3 GetWorldPosition() const { return worldPosition; }

        void SetNeighbourNode(TrafficNode* node);
        void RemoveNeighbourNode(TrafficNode* node);
        // std::vector<Vector3> GetChunksCrossedBySegment(Vector3 a, Vector3 b);

        void RenderDebug() const override;
        void SetDebugVisible(bool visible) override;
        bool IsDebugVisible() const override;

        friend class TrafficNetwork;
};

class RoadSegment {
    public: 
        const TrafficNode* start;
        const TrafficNode* end;

        void Render();
};

class TrafficNetwork {
    private:
        World* worldHandler;
        // std::vector<std::unique_ptr<TrafficNode>> nodes;
        std::vector<TrafficNode*> nodes;
        std::vector<RoadSegment*> roadSegments;
    
    // void GenerateFromRoadManager(RoadManager* roadManager);  // Convert tiles to nodes
    public: 
        TrafficNetwork(World* wrld); 
        TrafficNode* GetNearestNode(Vector3 position);
        std::vector<TrafficNode*> FindPath(TrafficNode* start, TrafficNode* end);
        TrafficNode* CreateNode(Vector3 position);
        void DeleteNode(TrafficNode* node);
        void DebugNodesIterator();

        RoadSegment* AddRoad(TrafficNode* node1, TrafficNode* node2);
        void DeleteRoad(TrafficNode* node1, TrafficNode* node2);
        ~TrafficNetwork();
};

#endif
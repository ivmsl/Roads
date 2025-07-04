#ifndef TRAFFIC_NODE_HPP_
#define TRAFFIC_NODE_HPP_

#include <vector>
#include "raylib.h"
#include "Game/Debug/DebugRenderer.hpp"
#include "Game/Rendering/RoadRenderer.hpp" // TEMP
#include "Game/World/World.hpp"
#include "Game/Core/Helpers.hpp"
#include <memory>

class World;
class DirtRoad;
class DirtIntersection;

enum class NodeType {
    NODE,
    SPAWNER_NODE,
    MIDDLE_NODE,
};

class TrafficNode : IDebuggable {
    protected: 
        Vector3 worldPosition;     // 3D position in world space
        Vector3 chunkInfo;
        std::vector<TrafficNode*> connections;  // Adjacent nodes
        float speedLimit;

        Material* roadMaterial;
        Mesh intMesh;       
        bool meshGenerated = false; 

        NodeType type;


        bool canUTurn = false;
    
    public: 
        TrafficNode(Vector3 position) : worldPosition(position) {
            type = NodeType::NODE;
        };
        
        void SetChunkInfo(Vector3 chunk);
        Vector3 GetChunkInfo() const { return chunkInfo; };
        Vector3 GetWorldPosition() const { return worldPosition; }

        void SetNeighbourNode(TrafficNode* node);
        void RemoveNeighbourNode(TrafficNode* node);
        const std::vector<TrafficNode*>& ShowNeighbours() const {return connections;};
        // std::vector<Vector3> GetChunksCrossedBySegment(Vector3 a, Vector3 b);

        void Render() const;
        void GenerateMesh();
        void RenderDebug() const override;
        void SetDebugVisible(bool visible) override;
        bool IsDebugVisible() const override;

        NodeType GetType() {return type;}
        void ChangeType(NodeType newType) {
            type = newType;
        }

        ~TrafficNode();

        friend class TrafficNetwork;
};

class SpawnerNode : TrafficNode {

};

class RoadSegment {
    public: 
        TrafficNode* start;
        TrafficNode* end;

        Material* roadMaterial;
        Mesh roadMesh;        // NEW: Store the actual road geometry
        bool meshGenerated = false;  // NEW: Track if mesh is built

        void Render();
        void GenerateMesh();
        bool IsVisibleToCamera(Camera3D* camera);
        ~RoadSegment();
};

class TrafficNetwork {
    private:
        World* worldHandler;
        // std::vector<std::unique_ptr<TrafficNode>> nodes;
        std::vector<TrafficNode*> nodes;
        std::vector<RoadSegment*> roadSegments;

        DirtRoad* dirtRoadRenderer;
        DirtIntersection* dirtIntersectionRenderer;
    
    // void GenerateFromRoadManager(RoadManager* roadManager);  // Convert tiles to nodes
    public: 
        TrafficNetwork(World* wrld); 
        TrafficNode* GetNearestNode(Vector3 position);
        std::vector<TrafficNode*>& FindPath(TrafficNode* start, TrafficNode* end);
        TrafficNode* CreateNode(Vector3 position);
        void DeleteNode(TrafficNode* node);
        void DebugNodesIterator(Camera3D* camera);
        const std::vector<TrafficNode*>& GetConnections(TrafficNode* node);

        RoadSegment* AddRoad(TrafficNode* node1, TrafficNode* node2);
        void DeleteRoad(TrafficNode* node1, TrafficNode* node2);
        void DeleteRoad(RoadSegment* road);


        bool ShouldRender(Vector3 startPos, Vector3 endPos, Camera3D camera, float maxDistance = 500.0f);
        ~TrafficNetwork();
};

#endif
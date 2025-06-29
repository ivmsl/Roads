#ifndef WORLD_HPP_
#define WORLD_HPP_

#include "raylib.h"
#include <vector>
#include "Game/Traffic/TrafficNode.hpp"
#include <unordered_map>
#include "Game/Core/Helpers.hpp"
#include "TerrainGen.hpp"

class TrafficNode;
class RoadSegment;
class Chunk;

class World {
    private:
        float chunkSize = 10.0f * 10.0f;
        std::unordered_map<Vector3, std::vector<TrafficNode*>> nodes;
        std::unordered_map<Vector3, std::vector<RoadSegment*>> roads;
        std::unordered_map<Vector3, Chunk*> chunks;
        // (chunk): [node1, node2, node3];
        TerrainGenerator* terrGen;
    public:
        World() { 
            terrGen = new TerrainGenerator{}; 
            RegisterChunk(GetChunkForPosition({2.0f, 2.0f, 2.0f}));
            RegisterChunk(GetChunkForPosition({-2.0f, 2.0f, -2.0f}));
            RegisterChunk(GetChunkForPosition({-2.0f, 2.0f, 2.0f}));
            RegisterChunk(GetChunkForPosition({2.0f, 2.0f, -2.0f}));
        };
        ~World() { if (terrGen) delete terrGen; };


        Vector3 GetChunkForPosition(Vector3 worldPosition);
        BoundingBox GetChunkBorders(Vector3 chunk);
        
        TrafficNode* RegisterTrafficNode(TrafficNode* node);
        TrafficNode* DeregisterTrafficNode(TrafficNode* node);

        TrafficNode* FindNearestNode(Vector3 worldPosition, float maxRadius = 8.0f);

        RoadSegment* RegisterRoad(RoadSegment* road);
        RoadSegment* DeregisterRoad(RoadSegment* road);

        RoadSegment* FindNearestRoad(Vector3 worldPosition, float maxRadius = 8.0f);

        std::vector<Vector3> GetChunksCrossedBySegment(Vector3 start, Vector3 end);


        float GetHeightForPos(Vector3 position);
        void RegisterChunk(Vector3 chunkID);
        void RenderChunks(); 
};      


class Chunk {
    private:
        float chunkLen = 10.0f;
        Vector3 chunkID  = {};
        // std::vector<TrafficNode*> nodes;
        // std::vector<RoadSegment*> roads;
        Mesh chunkMesh;
        Material* chunkMaterial;
        World* worldHandler;
        bool meshGenerated;
    public:
        Chunk(Vector3 chunkPos, World* wh, TerrainGenerator* tg) : 
            chunkID(chunkPos), chunkMaterial(tg->GetMaterial()), 
            worldHandler(wh) {};
        void GenerateMeshForChunk(TerrainGenerator* tg);
        void Render();
        
        ~Chunk();
        
};  

#endif
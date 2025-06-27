#pragma once

#include "raylib.h"
#include "raymath.h"
#include "Game/Traffic/TrafficNode.hpp"

class TrafficNode;

class RoadRenderer {
   public:
    static Mesh GenerateRoadMesh(Vector3 start, Vector3 end, float width = 8.0f);
    Material* GetRoadMaterial() { return &roadMaterial; }
    
    protected:
        Texture2D roadTexture;
        Material roadMaterial;
        
        static void CreateRoadStrip(Vector3 start, Vector3 end, float width, 
                                Vector3* vertices, Vector2* texCoords, 
                                unsigned short* indices);

        void RenderRoadMesh(Mesh roadMesh, Material dirtMaterial);
        

};

class DirtRoad : public RoadRenderer {
    public: 
        DirtRoad();
        ~DirtRoad();
};

class IntersectionRenderer {
    public: 
        static Mesh GenerateIntersectionMesh(Vector3 nodePosition, std::vector<TrafficNode*> connections, float roadWidth = 8.0f);
        static Mesh GenerateRoadEndMesh(Vector3 nodePosition, TrafficNode* connectedNode, float roadWidth = 8.0f);
        static Mesh GenerateStraightIntersectionMesh(Vector3 nodePosition, TrafficNode* node1, TrafficNode* node2, float roadWidth = 8.0f);
        static Mesh GenerateComplexIntersectionMesh(Vector3 nodePosition, 
                                               std::vector<TrafficNode*> connections, 
                                               float roadWidth = 8.0f);
        Material* GetRoadMaterial() { return &roadMaterial; }
    protected:
        Texture2D roadTexture;
        Material roadMaterial;
        static void CreateIntersectionPolygon(Vector3 center, 
                                        std::vector<Vector3>& roadEndPoints,
                                        std::vector<Vector3>& vertices,
                                        std::vector<unsigned short>& indices);

        static Mesh CreateMeshFromData(const std::vector<Vector3>& vertices,
                                     const std::vector<Vector2>& texCoords,
                                     const std::vector<unsigned short>& indices);

};      

class DirtIntersection : public IntersectionRenderer {
    public:
        DirtIntersection();
        ~DirtIntersection();
};
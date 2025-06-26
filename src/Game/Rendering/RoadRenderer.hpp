#pragma once

#include "raylib.h"
#include "raymath.h"

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
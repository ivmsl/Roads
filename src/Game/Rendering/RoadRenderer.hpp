#pragma once

#include "raylib.h"
#include "raymath.h"

class RoadRenderer {
   public:
    static Mesh GenerateRoadMesh(Vector3 start, Vector3 end, float width = 8.0f);
    
    private:
        static void CreateRoadStrip(Vector3 start, Vector3 end, float width, 
                                Vector3* vertices, Vector2* texCoords, 
                                unsigned short* indices);

        void RenderRoadMesh(Mesh roadMesh, Material dirtMaterial);
        void GetMaterial();

};
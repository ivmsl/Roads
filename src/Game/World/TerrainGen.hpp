#pragma once

// #include "raymath.h"
#include "raylib.h"
#include <vector>

class TerrainGenerator {
    private:
        float heightScale = 8.0f;  
        // float noiseScale = 0.01f; 
        float baseNoiseScale = 0.005f; 
        float detailScale = 0.02f;     
        Texture texture;
        Material material;
    
    public:
        TerrainGenerator();
        ~TerrainGenerator();

        float GetHeightAtPosition(float x, float z);
        std::vector<float> GenerateHeightmapForChunk(Vector3 chunkID, int resolution = 11);
        Mesh GenerateTerrainMesh(Vector3 chunkID, std::vector<float> heightData, int resolution = 11);
        Mesh GenerateTerrainMesh(Vector3 chunkID, int resolution = 11);

        Material* GetMaterial() { return &material; };

};
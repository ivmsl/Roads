#include "TerrainGen.hpp"


TerrainGenerator::TerrainGenerator() {
    texture = LoadTexture("resources/tex/GRASS.png");
    if (texture.id == 0) {
        TraceLog(LOG_ERROR, "TEXTURE: Failed to load grass.png!");
    } else {
        TraceLog(LOG_INFO, "TEXTURE: ID=%d, Width=%d, Height=%d, Format=%d", 
             texture.id, texture.width, texture.height, texture.format);
    }
    material = LoadMaterialDefault();
    SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, texture);
}

TerrainGenerator::~TerrainGenerator() {
    UnloadTexture(texture);
    UnloadMaterial(material);
}

float TerrainGenerator::GetHeightAtPosition(float x, float z) {
        
    float largeWaves = sin(x * baseNoiseScale) * cos(z * baseNoiseScale * 0.7f);
    float mediumWaves = sin(x * baseNoiseScale * 4) * cos(z * baseNoiseScale * 3) * 0.3f;
    float smallDetails = sin(x * detailScale) * cos(z * detailScale * 1.3f) * 0.1f;
    float combinedNoise = (largeWaves + mediumWaves + smallDetails) * heightScale;
    return combinedNoise; // Range: roughly -2.8 to +2.8 meters
}

std::vector<float> TerrainGenerator::GenerateHeightmapForChunk(Vector3 chunkID, int resolution) {
    
    std::vector<float> heights;
    heights.reserve(resolution * resolution);
    
    float chunkSize = 100.0f; // 10 tiles * 10 meters
    float stepSize = chunkSize / (resolution - 1);
    
    for (int z = 0; z < resolution; z++) {
        for (int x = 0; x < resolution; x++) {
            float worldX = chunkID.x * chunkSize + x * stepSize;
            float worldZ = chunkID.z * chunkSize + z * stepSize;
            heights.push_back(GetHeightAtPosition(worldX, worldZ));
        }
    }
    return heights;
}

Mesh GenerateTerrainMesh(Vector3 chunkID, std::vector<float> heightData, int resolution) {
    Mesh mesh = {};
    
    // Calculate mesh properties
    int vertCount = resolution * resolution;
    int triCount = (resolution - 1) * (resolution - 1) * 2; // 2 triangles per quad
    
    mesh.vertexCount = vertCount;
    mesh.triangleCount = triCount;
    
    // Allocate memory
    mesh.vertices = (float*)MemAlloc(vertCount * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(vertCount * 2 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(triCount * 3 * sizeof(unsigned short));
    
    float chunkSize = 100.0f;
    float stepSize = chunkSize / (resolution - 1);
    
    // Generate vertices
    for (int z = 0; z < resolution; z++) {
        for (int x = 0; x < resolution; x++) {
            int vertIndex = z * resolution + x;
            int heightIndex = z * resolution + x;
            
            // World position
            mesh.vertices[vertIndex * 3 + 0] = chunkID.x * chunkSize + x * stepSize;
            mesh.vertices[vertIndex * 3 + 1] = heightData[heightIndex];
            mesh.vertices[vertIndex * 3 + 2] = chunkID.z * chunkSize + z * stepSize;
            
            // Texture coordinates (repeating pattern)
            mesh.texcoords[vertIndex * 2 + 0] = (float)x / (resolution - 1) * 4.0f; // 4x repeat
            mesh.texcoords[vertIndex * 2 + 1] = (float)z / (resolution - 1) * 4.0f;
        }
    }
    
    // Generate triangle indices
    int triIndex = 0;
    for (int z = 0; z < resolution - 1; z++) {
        for (int x = 0; x < resolution - 1; x++) {
            // Current quad corners
            int topLeft = z * resolution + x;
            int topRight = z * resolution + (x + 1);
            int bottomLeft = (z + 1) * resolution + x;
            int bottomRight = (z + 1) * resolution + (x + 1);
            
            // Triangle 1: top-left, bottom-left, top-right
            mesh.indices[triIndex * 3 + 0] = topLeft;
            mesh.indices[triIndex * 3 + 1] = bottomLeft;
            mesh.indices[triIndex * 3 + 2] = topRight;
            triIndex++;
            
            // Triangle 2: top-right, bottom-left, bottom-right
            mesh.indices[triIndex * 3 + 0] = topRight;
            mesh.indices[triIndex * 3 + 1] = bottomLeft;
            mesh.indices[triIndex * 3 + 2] = bottomRight;
            triIndex++;
        }
    }
    
    UploadMesh(&mesh, false);
    return mesh;
};

Mesh TerrainGenerator::GenerateTerrainMesh(Vector3 chunkID, int resolution) {
    Mesh mesh = {};
    std::vector<float> heightData = GenerateHeightmapForChunk(chunkID);

    // Calculate mesh properties
    int vertCount = resolution * resolution;
    int triCount = (resolution - 1) * (resolution - 1) * 2; // 2 triangles per quad
    
    mesh.vertexCount = vertCount;
    mesh.triangleCount = triCount;
    
    // Allocate memory
    mesh.vertices = (float*)MemAlloc(vertCount * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(vertCount * 2 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(triCount * 3 * sizeof(unsigned short));
    
    float chunkSize = 100.0f;
    float stepSize = chunkSize / (resolution - 1);
    
    // Generate vertices
    for (int z = 0; z < resolution; z++) {
        for (int x = 0; x < resolution; x++) {
            int vertIndex = z * resolution + x;
            int heightIndex = z * resolution + x;
            
            // World position
            mesh.vertices[vertIndex * 3 + 0] = chunkID.x * chunkSize + x * stepSize;
            mesh.vertices[vertIndex * 3 + 1] = heightData[heightIndex];
            mesh.vertices[vertIndex * 3 + 2] = chunkID.z * chunkSize + z * stepSize;
            
            // Texture coordinates (repeating pattern)
            mesh.texcoords[vertIndex * 2 + 0] = (float)x / (resolution - 1) * 4.0f; // 4x repeat
            mesh.texcoords[vertIndex * 2 + 1] = (float)z / (resolution - 1) * 4.0f;
        }
    }
    
    // Generate triangle indices
    int triIndex = 0;
    for (int z = 0; z < resolution - 1; z++) {
        for (int x = 0; x < resolution - 1; x++) {
            // Current quad corners
            int topLeft = z * resolution + x;
            int topRight = z * resolution + (x + 1);
            int bottomLeft = (z + 1) * resolution + x;
            int bottomRight = (z + 1) * resolution + (x + 1);
            
            // Triangle 1: top-left, bottom-left, top-right
            mesh.indices[triIndex * 3 + 0] = topLeft;
            mesh.indices[triIndex * 3 + 1] = bottomLeft;
            mesh.indices[triIndex * 3 + 2] = topRight;
            triIndex++;
            
            // Triangle 2: top-right, bottom-left, bottom-right
            mesh.indices[triIndex * 3 + 0] = topRight;
            mesh.indices[triIndex * 3 + 1] = bottomLeft;
            mesh.indices[triIndex * 3 + 2] = bottomRight;
            triIndex++;
        }
    }
    
    UploadMesh(&mesh, false);
    return mesh;
};


#include "RoadRenderer.hpp"

// CreateRoadStrip() - Helper for complex road generation later
void RoadRenderer::CreateRoadStrip(Vector3 start, Vector3 end, float width, 
                                   Vector3* vertices, Vector2* texCoords, 
                                   unsigned short* indices) {
    
    // Extract the vertex generation logic from GenerateRoadMesh()
    Vector3 direction = Vector3Normalize(Vector3Subtract(end, start));
    Vector3 perpendicular = Vector3CrossProduct(direction, {0, 1, 0});
    Vector3 halfWidth = Vector3Scale(perpendicular, width/2.0f);
    
    // Fill the passed arrays
    vertices[0] = Vector3Subtract(start, halfWidth);
    vertices[1] = Vector3Add(start, halfWidth);
    vertices[2] = Vector3Subtract(end, halfWidth);
    vertices[3] = Vector3Add(end, halfWidth);
    
    float roadLength = Vector3Distance(start, end);
    texCoords[0] = {0.0f, 0.0f};
    texCoords[1] = {1.0f, 0.0f};
    texCoords[2] = {0.0f, roadLength/10.0f};
    texCoords[3] = {1.0f, roadLength/10.0f};
    
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 1; indices[4] = 3; indices[5] = 2;
}

// RenderRoadMesh() - Wrapper for road-specific rendering
void RoadRenderer::RenderRoadMesh(Mesh roadMesh, Material dirtMaterial) {
    // Basic rendering - could add road-specific effects later
    DrawMesh(roadMesh, dirtMaterial, MatrixIdentity());
    
    // Future: Add wear patterns, puddles, tire tracks, etc.
}

// In RoadRenderer::GenerateRoadMesh()
Mesh RoadRenderer::GenerateRoadMesh(Vector3 start, Vector3 end, float width) {
    Mesh mesh = {};
    
    // Calculate road direction and perpendicular for width
    Vector3 vertices[4];
    Vector2 texCoords[4];
    unsigned short indices[6];
    
    // Use the helper function
    CreateRoadStrip(start, end, width, vertices, texCoords, indices);


    
    mesh.vertexCount = 4;
    mesh.triangleCount = 2;

    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short));
    
    // **COPY THE ACTUAL DATA** - This was missing!
    for (int i = 0; i < 4; i++) {
        mesh.vertices[i * 3 + 0] = vertices[i].x;
        mesh.vertices[i * 3 + 1] = vertices[i].y;
        mesh.vertices[i * 3 + 2] = vertices[i].z;
        
        mesh.texcoords[i * 2 + 0] = texCoords[i].x;
        mesh.texcoords[i * 2 + 1] = texCoords[i].y;
    }
    
    for (int i = 0; i < 6; i++) {
        mesh.indices[i] = indices[i];
    }    
    
    UploadMesh(&mesh, false);
    return mesh;
}


DirtRoad::DirtRoad() {
    roadTexture = LoadTexture("resources/tex/dirtV.jpg");
    if (roadTexture.id == 0) {
        TraceLog(LOG_ERROR, "TEXTURE: Failed to load dirtH.jpg!");
        TraceLog(LOG_ERROR, "TEXTURE: Check if file exists at: resources/tex/dirtH.jpg");
    } else {
    TraceLog(LOG_INFO, "TEXTURE: Successfully loaded dirt_road.png");
    TraceLog(LOG_INFO, "TEXTURE: ID=%d, Width=%d, Height=%d, Format=%d", 
             roadTexture.id, roadTexture.width, roadTexture.height, roadTexture.format);
}

    roadMaterial = LoadMaterialDefault();
    SetMaterialTexture(&roadMaterial, MATERIAL_MAP_ALBEDO, roadTexture);
}

DirtRoad::~DirtRoad() {
    UnloadTexture(roadTexture);
    UnloadMaterial(roadMaterial);
}
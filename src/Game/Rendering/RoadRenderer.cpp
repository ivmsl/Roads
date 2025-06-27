#include "RoadRenderer.hpp"
#include "Game/Core/Helpers.hpp"

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
    Vector3 cStart = start;
    Vector3 cEnd = end;
    Helpers::ShrinkLineSegment(cStart, cEnd, width);
    // Calculate road direction and perpendicular for width
    Vector3 vertices[4];
    Vector2 texCoords[4];
    unsigned short indices[6];
    
    // Use the helper function
    CreateRoadStrip(cStart, cEnd, width, vertices, texCoords, indices);


    
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




//*****************************
//*         INTERSECTIONS 
//*****************************

Mesh IntersectionRenderer::CreateMeshFromData(const std::vector<Vector3>& vertices,
                                             const std::vector<Vector2>& texCoords,
                                             const std::vector<unsigned short>& indices) {
    Mesh mesh = {};
    mesh.vertexCount = vertices.size();
    mesh.triangleCount = indices.size() / 3;
    
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(indices.size() * sizeof(unsigned short));
    
    // Copy vertex data
    for (u_long i = 0; i < vertices.size(); i++) {
        mesh.vertices[i * 3 + 0] = vertices[i].x;
        mesh.vertices[i * 3 + 1] = vertices[i].y;
        mesh.vertices[i * 3 + 2] = vertices[i].z;
        
        mesh.texcoords[i * 2 + 0] = texCoords[i].x;
        mesh.texcoords[i * 2 + 1] = texCoords[i].y;
    }
    
    for (u_long i = 0; i < indices.size(); i++) {
        mesh.indices[i] = indices[i];
    }
    
    UploadMesh(&mesh, false);
    return mesh;
}

Mesh IntersectionRenderer::GenerateRoadEndMesh(Vector3 nodePosition, TrafficNode* connectedNode, float roadWidth) {
    Vector3 center = nodePosition;
    Vector3 end = connectedNode->GetWorldPosition();
    
    Vector3 connectionDirection = Vector3Normalize(Vector3Subtract(end, center));
    Vector3 perpendicular = Vector3Normalize(Vector3CrossProduct(connectionDirection, {0, 1, 0}));
    Vector3 scaledPerpendicular = Vector3Scale(perpendicular, roadWidth / 2.0f);

    Vector3 shiftToDirection = Vector3Add(center, Vector3Scale(connectionDirection, roadWidth / 2));

    std::vector<Vector3> vertices;
    std::vector<Vector2> texCoords;
    std::vector<unsigned short> indices;

    // Center vertex
    vertices.push_back(center);
    texCoords.push_back({0.5f, 0.5f});

    int numArcPoints = 12;
    float radius = roadWidth / 2.0f;

    for (int i = 0; i <= numArcPoints; i++) {
        float angle = PI + (PI * i / numArcPoints);  // From -90 to +90 degrees

        // Create local 2D arc point (in road space)
        float localX = cosf(angle) * radius;
        float localZ = sinf(angle) * radius;

        // Rotate into world space using the perpendicular and direction vectors
        Vector3 pointOffset = Vector3Add(
            Vector3Scale(perpendicular, localX),
            Vector3Scale(connectionDirection, localZ)
        );

        Vector3 arcPoint = Vector3Add(center, pointOffset);
        vertices.push_back(arcPoint);

        float u = 0.5f + localX / roadWidth;
        float v = 0.5f + localZ / roadWidth;
        texCoords.push_back({u, v});
    }
    // Generate fan triangles
    u_long i = 1;
    for (; i < vertices.size() - 1; i++) {
        indices.push_back(0); // center
        indices.push_back(i);
        indices.push_back(i + 1);
        TraceLog(LOG_DEBUG, "Triangle: {%d %d %d}", 0, i, i + 1);
    }

    TraceLog(LOG_DEBUG, "SIZES: %ul %ul %ul", vertices.size(), indices.size(), texCoords.size());
    //Additional points!!!
    //(i + 1) — last node
    Vector3 new1 = Vector3Add(vertices.at(1), Vector3Scale(connectionDirection, roadWidth));
    Vector3 new2 = Vector3Add(vertices.at(i), Vector3Scale(connectionDirection, roadWidth));

    texCoords.push_back({new1.x, new1.z});
    texCoords.push_back({new2.x, new2.z});
    
    vertices.push_back(new1);
    vertices.push_back(new2);
    
    for (const auto& vertex : vertices ) {
        TraceLog(LOG_DEBUG, "Current point: (%.2f, %.2f, %.2f)", vertex.x, vertex.y, vertex.z);
    }

    indices.push_back(i);
    indices.push_back(i + 2);
    indices.push_back(i + 1);

    indices.push_back(1);
    indices.push_back(i);
    indices.push_back(i + 1);
    
    



    // indices.push_back(i + 3);
    // indices.push_back(1);
    // indices.push_back(i + 1);
    // Vector3 additionalPoint;

    // vertices.push_back()
    // shiftToDirection
    

    return CreateMeshFromData(vertices, texCoords, indices);
}


Mesh IntersectionRenderer::GenerateStraightIntersectionMesh(Vector3 nodePosition, 
                                                           TrafficNode* node1, 
                                                           TrafficNode* node2, 
                                                           float roadWidth) {
    Vector3 center = nodePosition;
    
    // Calculate directions to both connected nodes
    Vector3 dir1 = Vector3Normalize(Vector3Subtract(node1->GetWorldPosition(), center));
    Vector3 dir2 = Vector3Normalize(Vector3Subtract(node2->GetWorldPosition(), center));
    
    // Create perpendicular vector
    Vector3 perpendicular = Vector3CrossProduct(dir1, {0, 1, 0});
    Vector3 halfWidth = Vector3Scale(perpendicular, roadWidth / 2.0f);
    
    std::vector<Vector3> vertices;
    std::vector<Vector2> texCoords;
    std::vector<unsigned short> indices;
    
    // Create road connection points
    Vector3 end1 = Vector3Add(center, Vector3Scale(dir1, roadWidth));
    Vector3 end2 = Vector3Add(center, Vector3Scale(dir2, roadWidth));
    
    // Create 4 corner vertices for intersection rectangle
    vertices.push_back(Vector3Add(end1, halfWidth));    // 0: end1 right
    vertices.push_back(Vector3Subtract(end1, halfWidth)); // 1: end1 left  
    vertices.push_back(Vector3Add(end2, halfWidth));    // 2: end2 right
    vertices.push_back(Vector3Subtract(end2, halfWidth)); // 3: end2 left
    
    // Texture coordinates
    texCoords.push_back({1.0f, 0.0f});
    texCoords.push_back({0.0f, 0.0f});
    texCoords.push_back({1.0f, 1.0f});
    texCoords.push_back({0.0f, 1.0f});
    
    // Create two triangles for the rectangle
    indices = {0, 1, 2, 1, 3, 2};
    
    return CreateMeshFromData(vertices, texCoords, indices);
}




Mesh IntersectionRenderer::GenerateIntersectionMesh(Vector3 nodePosition, std::vector<TrafficNode*> connections, float roadWidth) {
    if (connections.size() == 0) {
        return {}; // No connections, no mesh
    }
    
    if (connections.size() == 1) {
        // Road end - generate end cap
        // TraceLog(LOG_DEBUG, "Generating end ");
        return GenerateRoadEndMesh(nodePosition, connections[0], roadWidth);
    }
    
    if (connections.size() == 2) {
        // Straight road - generate simple intersection rectangle
        // return GenerateStraightIntersectionMesh(nodePosition, connections[0], connections[1], roadWidth);
        return GenerateComplexIntersectionMesh(nodePosition, connections, roadWidth);
    }

    if (connections.size() > 2) {
        return GenerateComplexIntersectionMesh(nodePosition, connections, roadWidth);
    }

    return Mesh{};
}

void IntersectionRenderer::CreateIntersectionPolygon(Vector3 center, 
                                           std::vector<Vector3>& roadEndPoints,
                                           std::vector<Vector3>& vertices,
                                           std::vector<unsigned short>& indices) {
    
    // Sort road endpoints by angle around the center
    std::sort(roadEndPoints.begin(), roadEndPoints.end(), 
        [center](const Vector3& a, const Vector3& b) {
            float angleA = atan2(a.z - center.z, a.x - center.x);
            float angleB = atan2(b.z - center.z, b.x - center.x);
            return angleA < angleB;
        });
    
    // Add center point
    vertices.push_back(center);
    int centerIndex = 0;
    
    // Add road connection points
    for (const auto& point : roadEndPoints) {
        vertices.push_back(point);
    }
    
    // Create triangles (fan triangulation from center)
    for (u_long i = 0; i < roadEndPoints.size(); i++) {
        int current = i + 1;  // +1 because center is at index 0
        int next = (i + 1) % roadEndPoints.size() + 1;
        
        // Triangle: center -> current -> next
        indices.push_back(centerIndex);
        indices.push_back(current);
        indices.push_back(next);
    }
}

Mesh IntersectionRenderer::GenerateComplexIntersectionMesh(Vector3 nodePosition, 
                                                          std::vector<TrafficNode*> connections, 
                                                          float roadWidth) {

    TraceLog(LOG_DEBUG, "Generating intersections for %i connections with width %f", connections.size(), roadWidth);                                                

    Vector3 center = nodePosition;
    float halfWidth = roadWidth / 2.0f;
    
    // Create intersection polygon points
    std::vector<Vector3> polygonPoints;
    
    // Sort connections by angle first
    std::sort(connections.begin(), connections.end(), 
        [center](TrafficNode* a, TrafficNode* b) {
            Vector3 dirA = Vector3Subtract(a->GetWorldPosition(), center);
            Vector3 dirB = Vector3Subtract(b->GetWorldPosition(), center);
            float angleA = atan2(dirA.z, dirA.x);
            float angleB = atan2(dirB.z, dirB.x);
            return angleA < angleB;
        });

    Vector3 wypadkowyDir{0.0f, 0.0f, 0.0f};
    
    // For each pair of adjacent roads, calculate intersection corner
    for (u_long i = 0; i < connections.size(); i++) {

        Vector3 currCenter = center;
        Vector3 otherCenter = connections[i]->GetWorldPosition();


        //Directional vector — from CENTER to OTHER node 
        Vector3 currentDir = Vector3Normalize(
            Vector3Subtract(otherCenter, currCenter)
        );
        // // TraceLog(LOG_DEBUG, "Current direction (%.2f, %.2f, %.2f) \ 
        //                     from center (%.2f, %.2f, %.2f) \
        //                     to (%.2f, %.2f, %.2f)", currentDir.x, currentDir.y, currentDir.z, 
        //                                             center.x, center.y, center.z,
        //                                             otherCenter.x, otherCenter.y, otherCenter.z);
        // wypadkowyDir = Vector3Add(currentDir, wypadkowyDir);
        // TraceLog(LOG_DEBUG, "Wypadkowy dir: (%.2f, %.2f, %.2f)", wypadkowyDir.x, wypadkowyDir.y, wypadkowyDir.z);

        //Perpendicular vector — either side of
        Vector3 currentPerpendicular = Vector3Normalize(
            Vector3CrossProduct(currentDir, {.0f, 1.0f, .0f })
        );
        Vector3 scaledPerpendicular = Vector3Scale(currentPerpendicular, roadWidth / 2.0f);

        TraceLog(LOG_DEBUG, "Created perpendicular: (%.2f, %.2f, %.2f) and scaled to (%.2f, %.2f, %.2f)", 
                                currentPerpendicular.x, currentPerpendicular.y, currentPerpendicular.z, 
                                scaledPerpendicular.x, scaledPerpendicular.y, scaledPerpendicular.z);
        //Shift our vector to contact edge (and even furher, it will be important later)
        Vector3 shiftToDirection = Vector3Add(currCenter, Vector3Scale(currentDir, roadWidth));


        polygonPoints.push_back(Vector3Subtract(shiftToDirection, scaledPerpendicular));
        polygonPoints.push_back(Vector3Add(shiftToDirection, scaledPerpendicular));
        
        

    }

    polygonPoints.push_back(Vector3Subtract(center, Vector3Scale(wypadkowyDir, roadWidth / 2.0f)));

    
    // Create mesh from polygon
    std::vector<Vector3> vertices;
    std::vector<Vector2> texCoords;
    std::vector<unsigned short> indices;
    
    // Add center
    vertices.push_back(center);
    texCoords.push_back({0.5f, 0.5f});

    std::vector<Vector3> filteredPoints = Helpers::GetConvexHull(polygonPoints, center);

    wypadkowyDir = {0.0f, 0.0f, 0.0f};
    for (const auto& point : filteredPoints) {
        Vector3 currentDir = Vector3Normalize(
            Vector3Subtract(point, center)
        );
        wypadkowyDir = Vector3Add(wypadkowyDir, currentDir);
    }
    
    // filteredPoints.push_back(Vector3Subtract(center, Vector3Scale(wypadkowyDir, roadWidth / 2.0f)));
    
    // Add polygon points
    // TraceLog(LOG_DEBUG, "Filtered points");
    for (const auto& point : filteredPoints) {        
        vertices.push_back(point);
        // TraceLog(LOG_DEBUG, "Current point: (%.2f, %.2f, %.2f)", point.x, point.y, point.z);
        float u = (point.x - center.x) / roadWidth + 0.5f;
        float v = (point.z - center.z) / roadWidth + 0.5f;
        texCoords.push_back({u, v});
    }
    
    // Triangulate
    for (u_long i = 0; i < filteredPoints.size(); i++) {
        int current = i + 1;
        int next = (i + 1) % filteredPoints.size() + 1;
        // next = next == 0 ? next++ : next;

        // TraceLog(LOG_DEBUG, "Triangle: {%d %d %d}", 0, current, next);
        
        indices.push_back(0);
        indices.push_back(next);
        indices.push_back(current);
    }
    
    return CreateMeshFromData(vertices, texCoords, indices);
}


DirtIntersection::DirtIntersection() {
    roadTexture = LoadTexture("resources/tex/dirtI.png");
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

DirtIntersection::~DirtIntersection() {
    UnloadTexture(roadTexture);
    UnloadMaterial(roadMaterial);
}
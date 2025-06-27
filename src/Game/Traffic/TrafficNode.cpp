#include "TrafficNode.hpp"
#include "raylib.h"
#include "raymath.h"
#include "Game/Core/Helpers.hpp"

void TrafficNode::SetChunkInfo(Vector3 chunk) {
    chunkInfo = chunk;
}

void TrafficNode::SetNeighbourNode(TrafficNode* node) {
    connections.push_back(node);
    node->connections.push_back(this);
}

void TrafficNode::RemoveNeighbourNode(TrafficNode* node) {
    connections.erase(
        std::remove(connections.begin(), connections.end(), node),
        connections.end()
    );
}

void TrafficNode::RenderDebug() const {
    
    Vector3 size = {4.0f, 0.3f, 4.0f};
    Color color = BLUE;

    DrawCube(worldPosition, size.x, size.y, size.z, color);
    DrawCubeWires(worldPosition, size.x, size.y, size.z, WHITE);
    // DrawCylinder(worldPosition, 8.0f, 8.0f, 0.8f, 30, RED);

}

void TrafficNode::SetDebugVisible(bool visible) {
    if (visible) {}
    return;
}

bool TrafficNode::IsDebugVisible() const {
    return true;
}

void TrafficNode::GenerateMesh() {
    // TRACE_LOG("Fired mesh generation");
    TraceLog(LOG_DEBUG, "Fired mesh gen. Connections size: %i, mesh gen %i", connections.size(), meshGenerated);
    if (meshGenerated) {
        if (intMesh.vaoId == 0) {
            intMesh = IntersectionRenderer::GenerateIntersectionMesh(worldPosition, connections);        
        }
        else {
            UnloadMesh(intMesh);
            intMesh = IntersectionRenderer::GenerateIntersectionMesh(worldPosition, connections);        
            TraceLog(LOG_DEBUG, "New mesh ID: %i", intMesh.vaoId);
        }
        return;
    }

    intMesh = IntersectionRenderer::GenerateIntersectionMesh(worldPosition, connections);
    meshGenerated = true;
}

void TrafficNode::Render() const {
    if (meshGenerated) {
        DrawMesh(intMesh, *roadMaterial, MatrixIdentity());
    }
}

TrafficNode::~TrafficNode() {
    if (meshGenerated) UnloadMesh(intMesh);
}

//********************************************************
//*                    Traffic Network 
//********************************************************

TrafficNetwork::TrafficNetwork(World* wrd) {
    worldHandler = wrd;
    dirtRoadRenderer = new DirtRoad();
    dirtIntersectionRenderer = new DirtIntersection();

}

TrafficNode* TrafficNetwork::CreateNode(Vector3 position) {
    TrafficNode* newnode = new TrafficNode(position);; 
    nodes.push_back(newnode);
    TrafficNode* regNode = worldHandler->RegisterTrafficNode(newnode);
    
    
    // TRACE_COORD(position);
    TraceLog(LOG_DEBUG, "Created node at %.2f %.2f %.2f", position.x, position.y, position.z);

    if (!regNode) {
        nodes.pop_back(); 
        delete newnode;    
        return nullptr;
    }
    newnode->roadMaterial = dirtIntersectionRenderer->GetRoadMaterial();
    newnode->GenerateMesh();
    return regNode;
}

void TrafficNetwork::DeleteNode(TrafficNode* node) {
    if (!node) return;
    std::vector<TrafficNode*> connectionsCopy = node->connections;
    worldHandler->DeregisterTrafficNode(node);

    for (auto conn : connectionsCopy) {
        DeleteRoad(node, conn);
    };

    nodes.erase(
        std::remove(nodes.begin(), nodes.end(), node),
        nodes.end()
    );

    delete node;   
    
}

const std::vector<TrafficNode*>& TrafficNetwork::GetConnections(TrafficNode* node) {
    return node->connections;
}

RoadSegment* TrafficNetwork::AddRoad(TrafficNode* node1, TrafficNode* node2) {
    RoadSegment* rs = new RoadSegment();
    rs->start = node1;
    rs->end = node2;
    rs->GenerateMesh();
    rs->roadMaterial = dirtRoadRenderer->GetRoadMaterial();
    roadSegments.push_back(rs);
    node1->SetNeighbourNode(node2);
    worldHandler->RegisterRoad(rs);
    node1->GenerateMesh();
    node2->GenerateMesh();
    return rs;
}


void TrafficNetwork::DeleteRoad(TrafficNode* node1, TrafficNode* node2) {
    if (!node1 || !node2) {
        return;
    }
    
    if (node1 == node2) {
        return;
    }

     auto it = std::find_if(roadSegments.begin(), roadSegments.end(),
        [node1, node2](RoadSegment* segment) {
            return (segment->start == node1 && segment->end == node2) ||
                   (segment->start == node2 && segment->end == node1);
        });
    
    if (it == roadSegments.end()) {
        return;
    }

    RoadSegment* roadToDelete = *it;
    roadSegments.erase(it);
    
    // Remove neighbor connections
    node1->RemoveNeighbourNode(node2);
    node2->RemoveNeighbourNode(node1); 
    worldHandler->DeregisterRoad(roadToDelete);
    delete roadToDelete;
}

void TrafficNetwork::DebugNodesIterator(Camera3D* camera) {
    for (TrafficNode* node : nodes) { 
        if (ShouldRender(node->GetWorldPosition(), node->GetWorldPosition(), *camera)) {
            node->Render();
        }
        
    }

    for (RoadSegment* segment : roadSegments) {
        if (ShouldRender(segment->start->GetWorldPosition(), segment->end->GetWorldPosition(), *camera)){ 
            segment->Render();
        } else {
            TraceLog(LOG_DEBUG, "Gen stopped");
        }
    }
}

bool TrafficNetwork::ShouldRender(Vector3 startPos, Vector3 endPos, Camera3D camera, float maxDistance) {
    Vector3 roadCenter = {
        (startPos.x + endPos.x) / 2.0f,
        (startPos.y + endPos.y) / 2.0f,
        (startPos.z + endPos.z) / 2.0f
    };
    
    float distanceToCamera = Vector3Distance(camera.position, roadCenter);
    return distanceToCamera <= maxDistance;
}

TrafficNetwork::~TrafficNetwork() {
    for (auto segment : roadSegments) delete segment;
    for (auto node : nodes) delete node;
}

//********************************************************
//*                    Road segment 
//********************************************************

void RoadSegment::Render() {
    Vector3 startCoord =start->GetWorldPosition();
    Vector3 endCoord = end->GetWorldPosition();
    DrawLine3D(startCoord, endCoord, RED);
    // float dx = endCoord.x - startCoord.x;
    // float dz = endCoord.z - startCoord.z;
    DrawMesh(roadMesh, *roadMaterial, MatrixIdentity());
    // // Calculate total distance
    // float totalDistance = std::sqrt(dx * dx + dz * dz);
    // int numSteps = (int)(totalDistance / 10);

    // for (int i = 0; i < numSteps; i++)
    // {
    //     float t = (numSteps > 1) ? (float)i / (numSteps - 1) : 0.0f;
    //     Vector3 currentPos = {
    //         startCoord.x + dx * t,
    //         startCoord.y,  // Keep Y constant
    //         startCoord.z + dz * t
    //     };

    //     DrawCube(currentPos, 3.0f, 3.0f, 3.0f, YELLOW);


    // }
    

}

void RoadSegment::GenerateMesh() {
    if (meshGenerated) return;
    roadMesh = RoadRenderer::GenerateRoadMesh(start->GetWorldPosition(), end->GetWorldPosition());
    meshGenerated = true;
}

bool RoadSegment::IsVisibleToCamera(Camera3D* camera) {
    // Vector3 startPos = start->GetWorldPosition();
    // Vector3 endPos = end->GetWorldPosition();
    
    // // Simple bounding box check
    // Vector3 minBounds = {
    //     fmin(startPos.x, endPos.x) - 5.0f,
    //     -1.0f,
    //     fmin(startPos.z, endPos.z) - 5.0f
    // };
    // Vector3 maxBounds = {
    //     fmax(startPos.x, endPos.x) + 5.0f,
    //     1.0f,
    //     fmax(startPos.z, endPos.z) + 5.0f
    // };
    
    // BoundingBox roadBounds = {minBounds, maxBounds};
    
    // // Check if bounding box is in camera frustum
    // return GetCameraMatrix(*camera).m15 > 0; // Simplified check
}

RoadSegment::~RoadSegment() {
    if (meshGenerated) UnloadMesh(roadMesh);
}


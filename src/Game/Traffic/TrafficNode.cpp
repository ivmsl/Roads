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
    
    Vector3 size = {8.0f, 0.3f, 8.0f};
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

TrafficNetwork::TrafficNetwork(World* wrd) {
    worldHandler = wrd;
}

TrafficNode* TrafficNetwork::CreateNode(Vector3 position) {
    TrafficNode* newnode = new TrafficNode(position);; 
    nodes.push_back(newnode);
    TrafficNode* regNode = worldHandler->RegisterTrafficNode(newnode);
    
    TraceLog(LOG_DEBUG, "Node created ");
    TRACE_COORD(position);

    if (!regNode) {
        nodes.pop_back(); 
        delete newnode;    
        return nullptr;
    }
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
    delete roadToDelete;
}

void TrafficNetwork::DebugNodesIterator() {
    for (TrafficNode* node : nodes)
    {
        node->RenderDebug();
    }

    for (RoadSegment* segment : roadSegments) {
        segment->Render();
    }
    

}

void RoadSegment::Render() {
    Vector3 startCoord =start->GetWorldPosition();
    Vector3 endCoord = end->GetWorldPosition();
    DrawLine3D(startCoord, endCoord, RED);
    float dx = endCoord.x - startCoord.x;
    float dz = endCoord.z - startCoord.z;
    
    // Calculate total distance
    float totalDistance = std::sqrt(dx * dx + dz * dz);
    int numSteps = (int)(totalDistance / 10);

    for (int i = 0; i < numSteps; i++)
    {
        float t = (numSteps > 1) ? (float)i / (numSteps - 1) : 0.0f;
        Vector3 currentPos = {
            startCoord.x + dx * t,
            startCoord.y,  // Keep Y constant
            startCoord.z + dz * t
        };

        DrawCube(currentPos, 3.0f, 3.0f, 3.0f, YELLOW);


    }
    

}

RoadSegment* TrafficNetwork::AddRoad(TrafficNode* node1, TrafficNode* node2) {
    RoadSegment* rs = new RoadSegment();
    rs->start = node1;
    rs->end = node2;
    roadSegments.push_back(rs);
    node1->SetNeighbourNode(node2);

    return rs;
}

TrafficNetwork::~TrafficNetwork() {
    for (auto segment : roadSegments) delete segment;
    for (auto node : nodes) delete node;
}
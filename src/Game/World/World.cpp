#include "raylib.h"
#include "raymath.h"
#include "World.hpp"
#include <limits>

BoundingBox World::GetChunkBorders(Vector3 chunk) {

    // TraceLog(LOG_DEBUG, "Bounding box for chunk (%f %f %f)", chunk.x, chunk.y, chunk.z);
    Vector3 min = {
        chunk.x * chunkSize,
        0.0f,
        chunk.z * chunkSize
    };

    // TraceLog(LOG_DEBUG, "BordersMin: %f %f %f", min.x, min.y, min.z);

    Vector3 max = {
        (chunk.x + 1) * chunkSize,
        100.0f,
        (chunk.z + 1) * chunkSize
    };

    // TraceLog(LOG_DEBUG, "BordersMax: %f %f %f", max.x, max.y, max.z);

    return BoundingBox{min, max};
}


Vector3 World::GetChunkForPosition(Vector3 worldPosition) {
    return Vector3 {
        static_cast<float>(std::floor(worldPosition.x / chunkSize)),
        static_cast<float>(std::floor(worldPosition.y / chunkSize)),
        static_cast<float>(std::floor(worldPosition.z / chunkSize))
    };
}

TrafficNode* World::FindNearestNode(Vector3 worldPosition, float maxRadius) {
    
    Vector3 chunk = GetChunkForPosition(worldPosition);

    auto it = nodes.find(chunk);
    
    if (it == nodes.end() || it->second.empty()) {
        // TraceLog(LOG_DEBUG, "No nodes found???");
        return nullptr; // No nodes in this chunk
    }

    TrafficNode* nearestNode = nullptr;
    float minDistance = maxRadius;
    
    for (TrafficNode* node : it->second) {
        float distance = Vector3Distance(node->GetWorldPosition(), worldPosition);
        
        // TRACE_COORD(worldPosition);
        if (distance < minDistance) {
            minDistance = distance;
            nearestNode = node;
            TraceLog(LOG_DEBUG, "\n\nChecking traffic node: \n\tDistance:%f, minDistance:%f \n\tCoord: %f %f %f", distance, minDistance);
            TraceLog(LOG_DEBUG, "\t\tFound obstruction at: ");
            TRACE_COORD(nearestNode->GetWorldPosition());
        }
    }

    // TraceLog(LOG_DEBUG, "Checking was ended....");
    
    return nearestNode;

}

TrafficNode* World::RegisterTrafficNode(TrafficNode* node) {
    Vector3 chunk = GetChunkForPosition(node->GetWorldPosition());
    nodes[chunk].push_back(node);
    node->SetChunkInfo(chunk);
    return node;
}

TrafficNode* World::DeregisterTrafficNode(TrafficNode* node) {
    Vector3 chunk = node->GetChunkInfo();
    auto& chunkNodes = nodes[chunk];
    
    auto it = std::find(chunkNodes.begin(), chunkNodes.end(), node);
    if (it != chunkNodes.end()) {
        chunkNodes.erase(it);
    }
    
    return node;
}
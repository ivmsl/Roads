#include "raylib.h"
#include "raymath.h"
#include "World.hpp"
#include "Game/Core/Helpers.hpp"
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

std::vector<Vector3> World::GetChunksCrossedBySegment(Vector3 start, Vector3 end) {
    std::vector<Vector3> chunks;
    
    Vector3 startChunk = GetChunkForPosition(start);
    Vector3 endChunk = GetChunkForPosition(end);
    
    // If both points are in the same chunk, just return that chunk
    if (Vector3Equals(startChunk, endChunk)) {
        chunks.push_back(startChunk);
        return chunks;
    }
    
    // Calculate the distance and number of steps to sample along the line
    float distance = Vector3Distance(start, end);
    // Sample every quarter chunk size to ensure we don't miss any chunks
    int numSteps = (int)(distance / (chunkSize * 0.25f)) + 1;
    
    Vector3 lastChunk = {-999999, -999999, -999999}; // Invalid chunk coordinates
    
    for (int i = 0; i <= numSteps; i++) {
        float t = (numSteps > 0) ? (float)i / numSteps : 0.0f;
        Vector3 currentPos = Vector3Lerp(start, end, t);
        Vector3 chunk = GetChunkForPosition(currentPos);
        
        // Only add if it's different from the last chunk (avoid duplicates)
        if (!Vector3Equals(chunk, lastChunk)) {
            chunks.push_back(chunk);
            lastChunk = chunk;
        }
    }
    
    return chunks;
}

RoadSegment* World::FindNearestRoad(Vector3 worldPosition, float radius) {
    // std::vector<RoadSegment*> nearbyRoads;
    RoadSegment* nearestRoad = nullptr;
    Vector3 currChunk = GetChunkForPosition(worldPosition);
    auto it = roads.find(currChunk);
    if (it == roads.end() || it->second.empty()) {
        return nullptr;
    }
            
    for (RoadSegment* road : it->second) {
                    // Check if the road segment is actually within radius
        float distToRoad = Helpers::DistancePointToLineSegment(worldPosition, 
                            road->start->GetWorldPosition(), 
                            road->end->GetWorldPosition());
                        
        if (distToRoad <= radius) {
            nearestRoad = road;
        }

    }

    return nearestRoad;
}

RoadSegment* World::RegisterRoad(RoadSegment* road) {
    if (!road) return nullptr;
    
    // Get all chunks this road segment passes through
    std::vector<Vector3> chunksAffected = GetChunksCrossedBySegment(
        road->start->GetWorldPosition(), 
        road->end->GetWorldPosition()
    );
    
    // Register road in all affected chunks
    for (Vector3 chunk : chunksAffected) {
        roads[chunk].push_back(road);
    }
    
    TraceLog(LOG_DEBUG, "Road registered across %d chunks", (int)chunksAffected.size());
    
    return road;
}

RoadSegment* World::DeregisterRoad(RoadSegment* road) {
    if (!road) return nullptr;
    
    // Get all chunks this road was registered in
    std::vector<Vector3> chunksAffected = GetChunksCrossedBySegment(
        road->start->GetWorldPosition(), 
        road->end->GetWorldPosition()
    );
    
    // Remove road from all affected chunks
    for (Vector3 chunk : chunksAffected) {
        auto& chunkRoads = roads[chunk];
        auto it = std::find(chunkRoads.begin(), chunkRoads.end(), road);
        if (it != chunkRoads.end()) {
            chunkRoads.erase(it);
        }
        
        // Clean up empty chunk entries
        if (chunkRoads.empty()) {
            roads.erase(chunk);
        }
    }
    
    return road;
}
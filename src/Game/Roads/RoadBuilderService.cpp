#include "raylib.h"
#include "RoadBuilderService.hpp"

RoadBuilderService::RoadBuilderService(World* w, TrafficNetwork* nM) {
    world = w;
    networkManager = nM;
    // this->Reset();

    nodeSpacing = 5;
    segmentSpacing = 10;
}

void RoadBuilderService::BuildRoad(Vector3 startPos, Vector3 endPos) {

    Vector3 currentPos = startPos;
    Vector3 currChunk;
    TrafficNode* lastNode = nullptr;
    NodeHeadTailInfo helperInfo = NO_CONNECTION;

    float dx = endPos.x - startPos.x;
    float dy = endPos.y - startPos.y;
    float dz = endPos.z - startPos.z;
    
    
    // Calculate total distance
    float totalDistance = std::sqrt(dx * dx + dz * dz);
    int numSteps = (int)(totalDistance / segmentSpacing); //byl +1 ale koniec potem dodalismy

    int i = 0;
    helperInfo = CheckNodesStartFinish(startPos, endPos);
    currChunk = world->GetChunkForPosition(currentPos);

    if (helperInfo == NODE_START || helperInfo == NODE_STARTEND) {
        lastNode = world->FindNearestNode(startPos);
        i++;
    }
    
    for (; i < numSteps; i++) {
        float t = (numSteps > 1) ? (float)i / (numSteps - 1) : 0.0f; // t goes from 0 to 1
        // TraceLog(LOG_DEBUG, "Iteration %i", i);
        currentPos = {
            startPos.x + dx * t,
            startPos.y + dy * t,  // Keep Y constant
            startPos.z + dz * t
        };

        if (i % nodeSpacing == 0 && !world->FindNearestNode(currentPos) && (numSteps - i) > 3) {
            // TraceLog(LOG_DEBUG, "Started building process");
            TrafficNode* node = networkManager->CreateNode(currentPos);
            if (lastNode && node != lastNode) {
                networkManager->AddRoad(lastNode, node);
            }
            lastNode = node;
            // TraceLog(LOG_DEBUG, "Built road at iteration %i", i);
        }
       
        // TRACE_COORD(currentPos);
        
    }

    currChunk = world->GetChunkForPosition(currentPos);
    if (helperInfo ==  NODE_END || helperInfo == NODE_STARTEND) {
            // TraceLog(LOG_DEBUG, "Checking ending pos");
            TrafficNode* node = world->FindNearestNode(currentPos);
            if (lastNode && node != lastNode) {
                networkManager->AddRoad(lastNode, node);
            }
        } else {
            if (!world->FindNearestNode(currentPos)) {
                TrafficNode* node = networkManager->CreateNode(currentPos);
                if (lastNode && node != lastNode) {
                    networkManager->AddRoad(lastNode, node);
                }
            }
        }
    
    // Reset();
}


bool RoadBuilderService::CheckIfObstructedOnTheLine(Vector3 startPos, Vector3 endPos) {
    Vector3 currentPos = startPos;
    // Vector3 currChunk;

    float dx = endPos.x - startPos.x;
    float dy = endPos.y - startPos.y;
    float dz = endPos.z - startPos.z;
    
    
    // Calculate total distance
    float totalDistance = std::sqrt(dx * dx + dz * dz);
    int numSteps = (int)(totalDistance / segmentSpacing);

    for (int i = 1; i < (numSteps - 1); i++) {
        float t = (numSteps > 1) ? (float)i / (numSteps - 1) : 0.0f; // t goes from 0 to 1
        
        currentPos = {
            startPos.x + dx * t,
            startPos.y,  // Keep Y constant
            startPos.z + dz * t
        };
        
        // Vector3 currChunk = world->GetChunkForPosition(currentPos);
        if (world->FindNearestNode(currentPos) || world->FindNearestRoad(currentPos)) {
            TraceLog(LOG_DEBUG, "Found obstruction in the middle at step %d: %f %f %f", 
                    i, currentPos.x, currentPos.y, currentPos.z);
            return true;
        }
    }

    return false;

}

RoadBuilderService::NodeHeadTailInfo RoadBuilderService::CheckNodesStartFinish(Vector3 startPos, Vector3 endPos) {
     
    NodeHeadTailInfo conn = NO_CONNECTION;

    if (CheckIfObstructedOnTheLine(startPos, endPos)) {
        conn = (NodeHeadTailInfo)6;
        return conn;
    }
     
    // if (world->FindNearestRoad(startPos) || world->FindNearestRoad(endPos)) {
    //     conn = (NodeHeadTailInfo)6;
    // }

    //  Vector3 currChunk = world->GetChunkForPosition(startPos);
     if (world->FindNearestNode(startPos)) {
        conn = (NodeHeadTailInfo)1;
     }
     
    //  currChunk = world->GetChunkForPosition(endPos);
     if (world->FindNearestNode(endPos)) {
        conn = (NodeHeadTailInfo)((int)conn + 2);
     }

     return conn;
}

void RoadBuilderService::HandleDeletionAtPlace(Vector3 position) const {
    if (TrafficNode* node = world->FindNearestNode(position)) {
        networkManager->DeleteNode(node);
    }
    else if (RoadSegment* road = world->FindNearestRoad(position)) {
        networkManager->DeleteRoad(road);
    }
}
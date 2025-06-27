#pragma once

#include "raylib.h"
#include "Game/Traffic/TrafficNode.hpp"

class RoadBuilderService {
    private:
        // int length; //w blokach
        // TrafficNode* lastNode; //in line
        int nodeSpacing;
        int segmentSpacing;
        
        World* world;
        TrafficNetwork* networkManager;  

    public: 
        enum NodeHeadTailInfo {
            NO_CONNECTION = 0,
            NODE_START = 1,
            NODE_END = 2,
            NODE_STARTEND = 3,
            OBSTRUCTED = 6
        };
        RoadBuilderService(World* w, TrafficNetwork* nM);
        // void Reset();
        // void InitBuild(Vector3 startPos, Vector3 endPos);
        void BuildRoad(Vector3 startPos, Vector3 endPos);
        TrafficNode* CheckIfObstructed(Vector3 startPos, Vector3 endPos);
        bool CheckIfObstructedOnTheLine(Vector3 startPos, Vector3 endPos);
        NodeHeadTailInfo CheckNodesStartFinish(Vector3 startPos, Vector3 endPos);
        void HandleDeletionAtPlace(Vector3 position) const;
};


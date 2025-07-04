#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "Game/Traffic/TrafficNode.hpp"

class PathfindingService {
private:
    
    struct PathNode {
        TrafficNode* node;
        float gCost;      
        float hCost;      
        TrafficNode* parent;
        
        float fCost() const { return gCost + hCost; }
    };
    
    struct PathNodeCompare {
        bool operator()(const PathNode& a, const PathNode& b) const {
            return a.fCost() > b.fCost();
        }
    };
    
    
    float CalculateHeuristic(TrafficNode* from, TrafficNode* to);

    std::vector<TrafficNode*> ReconstructPath(
        const std::unordered_map<TrafficNode*, TrafficNode*>& cameFrom,
        TrafficNode* end
    );
    
public:
    
    std::vector<TrafficNode*> FindPath(TrafficNode* start, TrafficNode* end);
};

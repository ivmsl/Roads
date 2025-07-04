#include "raylib.h"
#include "raymath.h"
#include "Helpers.hpp"

namespace Helpers {
    float DistancePointToLineSegment(Vector3 point, Vector3 lineStart, Vector3 lineEnd) {
        Vector3 line = Vector3Subtract(lineEnd, lineStart);
        Vector3 pointToStart = Vector3Subtract(point, lineStart);
        
        // Calculate the projection parameter t
        float lineLength = Vector3DotProduct(line, line);
        if (lineLength == 0.0f) {
            // Line segment is actually a point
            return Vector3Distance(point, lineStart);
        }
        
        float t = Vector3DotProduct(pointToStart, line) / lineLength;
        
        t = fmaxf(0.0f, fminf(1.0f, t));
        
        Vector3 closestPoint = Vector3Add(lineStart, Vector3Scale(line, t));
        return Vector3Distance(point, closestPoint);
    }

    void ShrinkLineSegment(Vector3& start, Vector3& end, float shrinkDistance) {
        Vector3 direction = Vector3Subtract(end, start);
        float totalLength = Vector3Length(direction);
        
        float maxShrink = totalLength * 0.4f; // 20% w srodku
        float actualShrink = fmin(shrinkDistance, maxShrink);
        
        Vector3 unitDirection = Vector3Normalize(direction);
        start = Vector3Add(start, Vector3Scale(unitDirection, actualShrink));
        end = Vector3Subtract(end, Vector3Scale(unitDirection, actualShrink));
        
    }

   std::vector<Vector3> GetConvexHull(std::vector<Vector3> points, Vector3 center) {
    if (points.size() <= 3) return points;
    
    std::sort(points.begin(), points.end(), 
        [center](const Vector3& a, const Vector3& b) {
            float angleA = atan2(a.z - center.z, a.x - center.x);
            float angleB = atan2(b.z - center.z, b.x - center.x);
            return angleA < angleB;
        });
    
    std::vector<Vector3> hull;
    TraceLog(LOG_DEBUG, "Unfiltered points");
    // Graham scan algorithm (simplified for 2D)
    for (const auto& point : points) {
        TraceLog(LOG_DEBUG, "Current point: (%.2f, %.2f, %.2f)", point.x, point.y, point.z);
        // Remove points that make a "right turn" (interior points)
        while (hull.size() >= 2) {
            Vector3 p1 = hull[hull.size() - 2];
            Vector3 p2 = hull[hull.size() - 1];
            
            // Calculate cross product to determine turn direction
            Vector3 v1 = Vector3Subtract(p2, p1);
            Vector3 v2 = Vector3Subtract(point, p2);
            
            float cross = v1.x * v2.z - v1.z * v2.x;  // 2D cross product
            
            if (cross <= 0) {  // Right turn or collinear - remove p2
                hull.pop_back();
            } else {
                break;  // Left turn - keep p2
            }
        }
        hull.push_back(point);
    }
    
    return hull;
}
}


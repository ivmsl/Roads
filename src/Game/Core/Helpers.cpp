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
        
        // Clamp t to [0, 1] to stay within the line segment
        t = fmaxf(0.0f, fminf(1.0f, t));
        
        // Calculate the closest point on the line segment
        Vector3 closestPoint = Vector3Add(lineStart, Vector3Scale(line, t));
        
        // Return distance from point to closest point on segment
        return Vector3Distance(point, closestPoint);
    }
}


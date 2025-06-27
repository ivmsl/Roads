#ifndef HELPERS_GEN_HPP
#define HELPERS_GEN_HPP

#include "raylib.h"
#include <filesystem>
#include <functional>

namespace std {
    template <>
    struct hash<Vector3> {
        std::size_t operator()(const Vector3& v) const {
            std::hash<float> floatHasher;
            size_t hx = floatHasher(v.x);
            size_t hy = floatHasher(v.y);
            size_t hz = floatHasher(v.z);
            return ((hx ^ (hy << 1)) >> 1) ^ (hz << 1);
        }
    };
}

#define TRACE_COORD(X) TraceLog(LOG_DEBUG, "At x: %f y: %f z: %f", X.x, X.y, X.z)
#define TRACE_LOG_CRD(X, Y) TraceLog(LOG_DEBUG, "%s. At x: %f y: %f z: %f", Y, X.x, X.y, X.z)
#define TRACE_LOG(X) TraceLog(LOG_DEBUG, X)
// #define TRACE_COORD(X) TraceLog(LOG_DEBUG, "At x: %f y: %f", X.x, X.y)


// std::string GetExecutableDirectory() {

// }
namespace Helpers {
    float DistancePointToLineSegment(Vector3 point, Vector3 lineStart, Vector3 lineEnd);
    void ShrinkLineSegment(Vector3& start, Vector3& end, float shrinkDistance = 4.0f);
    std::vector<Vector3> GetConvexHull(std::vector<Vector3> points, Vector3 center);
}


#endif
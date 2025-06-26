#pragma once

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
// #define TRACE_COORD(X) TraceLog(LOG_DEBUG, "At x: %f y: %f", X.x, X.y)


// std::string GetExecutableDirectory() {

// }
#ifndef HELPERS_UI_
#define HELPERS_UI_

#include "Game/Roads/RoadManager.hpp"

enum class UIMode {
    IDLE,
    ROAD_BUILD,
    ROAD_DELETE,
    ROAD_UPDATE,
    DELETE,
    INFO_SELECT
};

struct SelectionMetadata {
    int length;
    RoadDirection direction;
    ///...
};

#endif
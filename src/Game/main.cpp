#include "raylib.h"
#include "Game/Core/GameLoop.hpp"

int main() {
    GameLoop game;
    SetTraceLogLevel(LOG_DEBUG);

    game.Initialize(800, 600);
    
    game.Run();

    game.Cleanup();
    return 0;

}
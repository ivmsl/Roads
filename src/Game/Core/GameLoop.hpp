#ifndef GAME_LOOP_HPP_
#define GAME_LOOP_HPP_

#include "raylib.h"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"
#include "Game/Roads/RoadManager.hpp"
#include "Game/UI/UIManager.hpp"
#include "InputHandler.hpp"


class GameLoop {
    private: 
        CameraController* camera;
        GridRenderer* grid;
        InputHandler* input;
        RoadManager* roadManager;
        UIManager* uiManager;
        bool isRunning;

    public: 
        void Initialize(int screenW, int screenH);  
        void Run();         
        void Update();      
        void Render();
        void RenderDebugInfo();      
        void Cleanup();     

};
#endif

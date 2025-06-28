#ifndef GAME_LOOP_HPP_
#define GAME_LOOP_HPP_

#include "raylib.h"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"
#include "Game/Roads/RoadBuilderService.hpp"
#include "Game/UI/UIManager.hpp"
#include "InputHandler.hpp"
#include "Game/World/World.hpp"
#include "Game/Traffic/TrafficNode.hpp"
#include "Stager.hpp"

class GameStage;
class InputHandler;
class UIManager;

class GameLoop {
    private: 
        GameStage* gameStageHandler;
        CameraController* camera;
        GridRenderer* grid;
        InputHandler* input;
        TrafficNetwork* trafficNetwork;
        UIManager* uiManager;
        World* worldHandler;
        RoadBuilderService* roadBuilder;
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

#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include "raylib.h"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"
#include "Game/UI/UIManager.hpp"
#include "Game/World/World.hpp"

class UIManager;

class InputHandler {
    public:
         enum EnabledHandles {
            CAMERA = 1,
            ROADPLACE = 1 << 1,
            MENU = 1 << 2
        };
    private:
        CameraController* cameraController;
        GridRenderer* gridRenderer;
        UIManager* uiManager;
        World* worldHandler;

        int enabledHandles;

        void HandleCameraInput();
        void HandleRoadPlacement();
        void HandleMenuSelection();
        // void HandleUIInput(); 
        // void HandleGameInput();
        float selectedHeight = 0.0f;
    
    public:
        void Initialize(CameraController* camera, GridRenderer* grid, UIManager* uim, World* wh);
        void ProcessInput();  // Main method called each frame
        void ChangeHeight(int dh);

        void EnableHandle(EnabledHandles handle);
        void DisableHandle(EnabledHandles handle);
};  

#endif
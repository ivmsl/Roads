#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include "raylib.h"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"
#include "Game/UI/UIManager.hpp"

class InputHandler {
    private:
        CameraController* cameraController;
        GridRenderer* gridRenderer;
        UIManager* uiManager;

        void HandleCameraInput();
        void HandleRoadPlacement();
        // void HandleUIInput(); 
        // void HandleGameInput();
    
    public:
        void Initialize(CameraController* camera, GridRenderer* grid, UIManager* uim);
        void ProcessInput();  // Main method called each frame

};

#endif
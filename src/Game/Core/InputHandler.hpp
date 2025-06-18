#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include "raylib.h"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"

class InputHandler {
    private:
        CameraController* cameraController;
        GridRenderer* gridRenderer;

        void HandleCameraInput();
        // void HandleUIInput(); 
        // void HandleGameInput();
    
    public:
        void Initialize(CameraController* camera, GridRenderer* grid);
        void ProcessInput();  // Main method called each frame

};

#endif
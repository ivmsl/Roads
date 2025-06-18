#ifndef INPUT_HANDLER_HPP_
#define INPUT_HANDLER_HPP_

#include "raylib.h"
#include "Game/Rendering/CameraController.hpp"
#include "Game/Rendering/GridRender.hpp"
#include "Game/Roads/RoadPlacement.hpp"

class InputHandler {
    private:
        CameraController* cameraController;
        GridRenderer* gridRenderer;
        RoadPlacement* roadPlacement;

        void HandleCameraInput();
        void HandleRoadPlacement();
        // void HandleUIInput(); 
        // void HandleGameInput();
    
    public:
        void Initialize(CameraController* camera, GridRenderer* grid, RoadPlacement* roadPlacement);
        void ProcessInput();  // Main method called each frame

};

#endif
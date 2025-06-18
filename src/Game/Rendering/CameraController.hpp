#ifndef CAMERA_CONTROLLER_HPP_
#define CAMERA_CONTROLLER_HPP_
#include "raylib.h"

class CameraController {
    private: 
        Camera3D camera;
        Vector3 targetPosition = {50.0f, 0.0f, 50.0f};  // Center of 100x100 map
        float tiltAngle = 50.0f; 
        float sideTilt = -20.0f;  
        float cameraDistance = 60.0f;


    public:
        void Initialize();          // set up orthographic camera
        void Update();              // handle pan/zoom
        //Vector3 ScreenToWorld();    // mouse to world coordinates
        Camera3D& GetCamera();       // for BeginMode3D()
        inline void RecalculateTilt();

};


#endif

#ifndef CAMERA_CONTROLLER_HPP_
#define CAMERA_CONTROLLER_HPP_
#include "raylib.h"

class CameraController {
    private: 
        Camera3D camera;
        Vector3 targetPosition = {50.0f, 0.0f, 50.0f};  
        float tiltAngle = 50.0f; 
        float sideTilt = -20.0f;  
        float cameraDistance = 60.0f;


    public:
        void Initialize();          
        void Update();              
        //Vector3 ScreenToWorld();   
        Camera3D& GetCamera();       
        inline void RecalculatePosition();
        void Pan(Vector2 movementDelta);
        void Zoom(float wheelDelta);
        void Rotate(float tiltFix);
};


#endif

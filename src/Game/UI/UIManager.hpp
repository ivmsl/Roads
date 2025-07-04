#ifndef UI_MANAGER_
#define UI_MANAGER_

#include "raylib.h"
#include "Game/Roads/RoadBuilderService.hpp"
#include "Game/Core/InputHandler.hpp"
#include "Game/Core/Stager.hpp"
#include "Game/Traffic/TrafficManager.hpp"
#include <functional>
#include "Helpers.hpp"
#include "UIMenu.hpp"

class GameStage;

class UIManager {
    public:

        UIManager(RoadBuilderService* rb, GameStage* sh, TrafficManager* tm);
        enum UIMode {
            IDLE,
            MAKE_NODES,
            SELECTION_SNAP,
            CHECK_OBST,
            ROAD_DELETE,
            ROAD_UPDATE,
            DELETE,
            INFO_SELECT
        };
        
        friend class InputHandler;
        // using TraceAlgorytm = std::function<void(Vector2 startGridPos, Vector2 currentGridPos, SelectionMetadata mtd, std::function<void(int x, int y, SelectionMetadata mtd)> action)>; //alias aby korzystać z nazwy TraceAlgorytm

    private:
        bool snap = false;
        
        // Vector2 height;
        UIMode mode;
        bool isSelecting = false;
        Vector3 startGridPos;
        Vector3 currentGridPos;
        RoadBuilderService* roadBuilder;
        GameStage* stagerHandler;
        TrafficManager* trafficManager;

        Color brickColor = WHITE;
        Menu* currentMenu;


        // int nodeSize; //SQUARE node size;


        // TraceAlgorytm selectedAlgorytm;
    public:
        //Selection group

        // void SetAlgorytm(TraceAlgorytm alg);
        void DrawTextInfo();
        void RenderMenu();
        void ActOnMenu();

        Vector2 ScreenToWorld(Ray ray);
        void StartSelection(Vector3 gridPos);
        void EndSelection();
        void UpdateSelection(Vector3 gridPos);
        void CancelSelection();
        bool IsSelecting() { return isSelecting; }
        
        void CompleteSelectionAction();

        //Render group 
        void RenderSelection();
        void RenderSelectionBrick(Vector3 position);

        void ModeSelect(UIMode newMode);

        void ToggleSnap();
        void HandleMenuSelection();

        //MODE — BUILD ROAD
        //MODE - DELETE

}; 

#endif
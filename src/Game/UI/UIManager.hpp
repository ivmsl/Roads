#ifndef UI_MANAGER_
#define UI_MANAGER_

#include "raylib.h"
#include "Game/Roads/RoadBuilderService.hpp"
#include <functional>
#include "Helpers.hpp"


class UIManager {
    public:

        UIManager(RoadBuilderService* rb);
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
        // using TraceAlgorytm = std::function<void(Vector2 startGridPos, Vector2 currentGridPos, SelectionMetadata mtd, std::function<void(int x, int y, SelectionMetadata mtd)> action)>; //alias aby korzystać z nazwy TraceAlgorytm

    private:
        bool snap = false;
        UIMode mode;
        bool isSelecting = false;
        Vector2 startGridPos;
        Vector2 currentGridPos;
        RoadBuilderService* roadBuilder;

        Color brickColor = WHITE;

        // int nodeSize; //SQUARE node size;


        // TraceAlgorytm selectedAlgorytm;
    public:
        //Selection group

        // void SetAlgorytm(TraceAlgorytm alg);
        void DrawTextInfo();

        Vector2 ScreenToWorld(Ray ray);
        void StartSelection(Vector2 gridPos);
        void EndSelection();
        void UpdateSelection(Vector2 gridPos);
        void CancelSelection();
        bool IsSelecting() { return isSelecting; }
        
        void CompleteSelectionAction();


        //Render group 
        void RenderSelection();
        void RenderSelectionBrick(int x, int y);

        void ModeSelect(UIMode newMode);

        void ToggleSnap();

        //MODE — BUILD ROAD
        //MODE - DELETE

}; 

#endif
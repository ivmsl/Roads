#ifndef UI_MANAGER_
#define UI_MANAGER_

#include "raylib.h"
#include "Game/Roads/RoadManager.hpp"
#include <functional>

enum class UIMode {
    IDLE,
    ROAD_BUILD,
    ROAD_DELETE,
    ROAD_UPDATE,
    DELETE,
    INFO_SELECT
};

class UIManager {
    public:

        UIManager(RoadManager* rm);
        using TraceAlgorytm = std::function<void(Vector2 startGridPos, Vector2 currentGridPos, RoadDirection direction, std::function<void(int x, int y, RoadDirection dir)> action)>; //alias aby korzystać z nazwy TraceAlgorytm

    private:
        UIMode mode;
        bool isSelecting = false;
        Vector2 startGridPos;
        Vector2 currentGridPos;
        RoadManager* roadManager;

        TraceAlgorytm selectedAlgorytm;
    public:
        //Selection group

        void SetAlgorytm(TraceAlgorytm alg);

        Vector2 ScreenToGrid(Ray ray);
        void StartSelection(Vector2 gridPos);
        void EndSelection();
        void UpdateSelection(Vector2 gridPos);
        void CancelSelection();
        bool IsSelecting() { return isSelecting; }
        
        void CompleteSelectionAction();


        //Render group 
        void RenderSelection();
        void RenderSelectionBrick(int x, int y, RoadDirection direction);

        inline RoadDirection DetermineRoadDirection();

        void ModeSelect(UIMode newMode);

        //MODE — BUILD ROAD
        // void BuildRoadPath(RoadManager& manager); //pass some algorythm....

        //MODE - DELETE

}; 

#endif
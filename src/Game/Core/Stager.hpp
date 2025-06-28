#pragma once
#include "Game/Core/GameLoop.hpp"

class Stager {};


class GameStage : public Stager {
    public:
         enum Stage {
            GAME_INIT,
            GAME_MENU,
            GAME_START,
            GAME_RUNNING,
            GAME_CLEANUP
        };
    private:
        Stage currentStage;
        int numberOfStages = 5;
    public:
        
        void ChangeStage(Stage newStage);
        void GoToNextStage();
        Stage GetCurrentStageInfo() { return currentStage; };
        friend class GameLoop;
    
};
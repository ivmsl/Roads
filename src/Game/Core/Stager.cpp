#include "Stager.hpp"

void GameStage::ChangeStage(Stage newStage) {
    currentStage = newStage;
}

void GameStage::GoToNextStage() {
    currentStage = (Stage) (((int)currentStage + 1) % numberOfStages);
}

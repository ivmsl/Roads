#include "raylib.h"
#include "raymath.h"
#include "UIManager.hpp"
#include <functional>
#include "Game/Core/Helpers.hpp"


UIManager::UIManager(RoadBuilderService* rb, GameStage* sh){

    isSelecting = false;
    startGridPos = {-1.0f, -1.0f, -1.0f};
    currentGridPos = {-1.0f, -1.0f, -1.0};
    roadBuilder = rb;
    stagerHandler = sh;
    currentMenu = new Menu;
}

Vector2 UIManager::ScreenToWorld(Ray ray) {

    // Find where ray hits the ground plane (y = 0)
    float t = -ray.position.y / ray.direction.y;
    Vector3 hitPoint = {
        ray.position.x + t * ray.direction.x,
        0.0f,
        ray.position.z + t * ray.direction.z
    };
    
    return {(float)hitPoint.x, (float)hitPoint.z};

}

void UIManager::StartSelection(Vector3 gridPos) {

    isSelecting = true;
    startGridPos = gridPos;
    currentGridPos = gridPos;
    
    TraceLog(LOG_DEBUG, "Selected grid at (%d, %d)", 
             (int)gridPos.x, (int)gridPos.z);
}

void UIManager::UpdateSelection(Vector3 gridPos) {
    if (!isSelecting) return;

    switch (mode)
    {
        case ROAD_DELETE: {
            startGridPos = gridPos;
            currentGridPos = gridPos;
            return;   
            break;
        }
        default:
            break;
    }


    currentGridPos = gridPos;

    // TraceLog(LOG_DEBUG, "\n\nUI Manager: tracing osbtruction;");


    if (snap) {
        Vector3 originalEndPos = currentGridPos;
        float dx = std::abs(startGridPos.x - currentGridPos.x);
        // float dy = std::abs(startGridPos.y - currentGridPos.y);
        float dz = std::abs(startGridPos.z - currentGridPos.z);
        float dxdz = std::abs(dx - dz);

        // TraceLog(LOG_DEBUG, "SNAP %f %f %f", dx, dy, dz);


        if (dx > dz && dxdz > 4.f) {
            currentGridPos.z = startGridPos.z;
            // TraceLog(LOG_DEBUG, "Horisontal line");
        } else if (dz > dx && dxdz > 4.f) {
            currentGridPos.x = startGridPos.x;
            // TraceLog(LOG_DEBUG, "Vertical line");
        }

        RoadBuilderService::NodeHeadTailInfo conn = roadBuilder->CheckNodesStartFinish(originalEndPos, originalEndPos);
        if (conn > 1 && conn < 4) currentGridPos = originalEndPos;
    }

    // Vector3 startPos = {startGridPos.x, 0.0f, startGridPos.y};
    // Vector3 endPos = {currentGridPos.x, 0.0f, currentGridPos.y};

    if (mode == MAKE_NODES) {
        RoadBuilderService::NodeHeadTailInfo conn = roadBuilder->CheckNodesStartFinish(startGridPos, currentGridPos);
        switch (conn)
        {
        case RoadBuilderService::NodeHeadTailInfo::NODE_START: {
            brickColor = YELLOW;
            break;
        }
        case RoadBuilderService::NodeHeadTailInfo::NODE_END: {
            brickColor = GREEN;
            break;
        }
        case RoadBuilderService::NodeHeadTailInfo::NODE_STARTEND:  {
            brickColor = MAGENTA;
            break;
        }
        case RoadBuilderService::OBSTRUCTED: {
            brickColor = RED;
            break;
        }
        default:
            brickColor = BLUE;
            break;
        }
    }

        
}

void UIManager::EndSelection() {
    if (!isSelecting) return;
    isSelecting = false;
    CompleteSelectionAction();
}

void UIManager::CompleteSelectionAction() {

    brickColor = WHITE;
    if (mode == UIMode::IDLE) {
        
    }
    
    if (mode == ROAD_DELETE) {
        
        // Vector3 position = {currentGridPos.x, 0.0f, currentGridPos.y};
        roadBuilder->HandleDeletionAtPlace(currentGridPos);
        return;
    }

    if (mode == UIMode::MAKE_NODES) {
        
        // Vector3 startPos = {startGridPos.x, 0.0f, startGridPos.y};
        // Vector3 endPos = {currentGridPos.x, 0.0f, currentGridPos.y};
        RoadBuilderService::NodeHeadTailInfo conn = roadBuilder->CheckNodesStartFinish(startGridPos, currentGridPos);

        if (!(conn == RoadBuilderService::OBSTRUCTED)) {
            TraceLog(LOG_DEBUG, "Start building road, conn status %i", conn);
            roadBuilder->BuildRoad(startGridPos, currentGridPos);
        } else {
            TraceLog(LOG_DEBUG, "Can not build! Error: path is abstructed");
        }
    }
}

void UIManager::CancelSelection() {
    if (!isSelecting) return;
    isSelecting = false;

    TraceLog(LOG_DEBUG, "Cancelled road placement");
}

void UIManager::RenderSelection() {
    if (!isSelecting) return;

    float dx = currentGridPos.x - startGridPos.x;
    float dy = currentGridPos.y - startGridPos.y;
    float dz = currentGridPos.z - startGridPos.z;

    if (mode == ROAD_DELETE) { RenderSelectionBrick(currentGridPos); return; };
    
    // Calculate total distance
    Vector3 renderPlace;
    float totalDistance = std::sqrt(dx * dx + dz * dz);

    int numSteps = (int)(totalDistance / 10); //byl +1 ale koniec potem dodalismy

     for (int i = 0; i < numSteps; i++) {
        float t = (numSteps > 1) ? (float)i / (numSteps - 1) : 0.0f; // t goes from 0 to 1
        
        renderPlace = {
            startGridPos.x + dx * t,
            startGridPos.y + dy * t,
            startGridPos.z + dz * t,
        };

        RenderSelectionBrick(renderPlace);
     }
}

void UIManager::RenderSelectionBrick(Vector3 position) {
    // Convert grid to world coordinates
    // float worldX = x * 10.0f + 5.0f;  // Center of tile
    // float worldZ = y * 10.0f + 5.0f;
    // float worldY = 2.0f;  // Slightly above regular roads
    
    Vector3 size;
    Color color;
    
    
    size = {10.0f, 1.0f, 10.0f};
    color = {255, 255, 255, 100}; 
    
    DrawCube(position, size.x, size.y, size.z, brickColor);
    DrawCubeWires(position, size.x, size.y, size.z, BLUE);
}

void UIManager::DrawTextInfo() {
    int yOffset = 160;  
    int lineHeight = 16;
    Color textColor = WHITE;
    Color labelColor = YELLOW;

    yOffset -= 3 * lineHeight;

    DrawText("=== Selected mode ===", 10, yOffset, 14, labelColor);
    yOffset += lineHeight;

    switch (mode)
    {
    case IDLE:
        DrawText("Idle", 10, yOffset, 14, WHITE);
        break;
    case MAKE_NODES:
        DrawText("Building", 10, yOffset, 14, WHITE);
        break;
    case DELETE:
        DrawText("Deleting road", 10, yOffset, 14, WHITE);
        break;
    
    default:
        break;
    }
    yOffset += lineHeight;
    DrawText(TextFormat("Snapping: %s", (snap) ? "on" : "off"), 
        10, yOffset, 14, WHITE);
    yOffset += lineHeight;

    DrawText("=== Game statistics ===", 10, yOffset, 14, labelColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("Current starting position: (%.2f, %.2f)", 
             startGridPos.x, startGridPos.y), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
    
    DrawText(TextFormat("End position:   (%.2f, %.2f)", 
             currentGridPos.x, currentGridPos.y), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;

    DrawText(TextFormat("CurrentHeight:   %.2f", currentGridPos.y),
            10, yOffset, 12, textColor);
    yOffset += lineHeight;



    float dx = startGridPos.x - currentGridPos.x;
    float dz = startGridPos.y - currentGridPos.y;
    
    // Calculate total distance
    float totalDistance = std::sqrt(dx * dx + dz * dz);
    int numSteps = (int)(totalDistance / 10.0f); //byl +1 ale koniec potem dodalismy


    DrawText(TextFormat("Length of the road in blocks:   (%i, %.2f)", 
             numSteps, totalDistance), 
             10, yOffset, 12, textColor);
    yOffset += lineHeight;
}

void UIManager::RenderMenu() {
    currentMenu->RenderMenu();
}

void UIManager::ModeSelect(UIMode newMode) {
    mode = newMode;
}

void UIManager::ToggleSnap() {
    snap = !snap;
}

void UIManager::HandleMenuSelection() {
    if (stagerHandler->GetCurrentStageInfo() == GameStage::GAME_MENU) {
        if (currentMenu->GetSelectedOption() == Menu::START_GAME) {
            stagerHandler->GoToNextStage();
        }
        else {
            stagerHandler->ChangeStage(GameStage::GAME_CLEANUP);
        }
        
    }
    
}
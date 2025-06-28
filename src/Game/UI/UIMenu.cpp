#include "UIMenu.hpp"

void Menu::RenderMenu() {

    yOffset = GetScreenHeight() / 5;
    int fontSize = 32;
    int len = MeasureText("Roads", fontSize);
    int centerX = GetScreenWidth() / 2;

    DrawText("Roads", centerX - (len / 2), yOffset, fontSize, textColor);
    yOffset += 32;

    fontSize = 14;
    len = MeasureText("Game prototype by IM, US 2025", fontSize);
    DrawText("Game prototype by IM, US 2025", centerX - (len / 2), yOffset, fontSize, textColor);


    yOffset = GetScreenHeight() / 5 * 3;
    fontSize = 24;
    len = MeasureText("Start Game", fontSize);
    DrawText("Start Game", centerX - (len / 2), yOffset, fontSize, options == START_GAME ? outlineColor : textColor);
    
    yOffset += 32;
    len = MeasureText("Quit", fontSize);
    DrawText("Quit", centerX - (len / 2), yOffset, fontSize, options == QUIT ? outlineColor : textColor);
}

void Menu::MenuUp() {
    TraceLog(LOG_DEBUG, "Up button pressed, current option: %i", (int) options);
    options = (SelectedOptions) (((int)options + 1) % totalOptions);
    TraceLog(LOG_DEBUG, "Up button pressed, current option after change: %i", (int) options);
}

void Menu::MenuDown() {
    options = (SelectedOptions) (((int)options + 1) % totalOptions);
}
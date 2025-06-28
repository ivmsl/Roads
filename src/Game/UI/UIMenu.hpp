#pragma once
#include "raylib.h"


class Menu {
    public: 
        enum SelectedOptions {
            START_GAME = 0,
            QUIT = 1,
        };
    private:
        SelectedOptions options = (SelectedOptions)0;
        int totalOptions = 2;
    protected:
        int yOffset;  
        int lineHeight;
        
        Color textColor = WHITE;
        Color textSubColot = BLACK;
        Color outlineColor = YELLOW;
        Color newColor = YELLOW;

    public:
        void RenderMenu();
        SelectedOptions GetSelectedOption() { return options; };
        void MenuUp();
        void MenuDown();
};

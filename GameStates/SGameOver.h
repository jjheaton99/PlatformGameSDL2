#pragma once

#include "GameState.h"
#include "MenuButton.h"

class SGameOver : public GameState
{
public:
    enum GameOverSelection
    {
        PLAY_AGAIN,
        QUIT,
        NONE
    };

private:
    MenuButton m_playAgain;
    MenuButton m_quit;

    GameOverSelection m_currentSelection{ PLAY_AGAIN };

    TextureObject m_gameOverTexture{"Assets/MiscTextures/gameOver.png"};

    int m_buttonWidth;
    int m_buttonHeight;

    int m_bigButtonWidth;
    int m_bigButtonHeight;

    bool gameOverControls(SDL_Event& event);

    void cycleUp();
    void cycleDown();
    bool m_joyStickCentered{ true };

public:
    SGameOver();
    ~SGameOver();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};


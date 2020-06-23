#pragma once

#include "GameState.h"

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
    MenuButton m_playAgain{ "Assets/MenuButtons/playAgain.png", "Assets/MenuButtons/playAgainS.png", 96, 14 };
    MenuButton m_quit{ "Assets/MenuButtons/quit.png", "Assets/MenuButtons/quitS.png", 60, 14 };

    GameOverSelection m_currentSelection{ PLAY_AGAIN };

    TextureObject m_gameOverTexture{"Assets/MiscTextures/gameOver.png"};

    int m_buttonWidth;
    int m_buttonHeight;

    int m_bigButtonWidth;
    int m_bigButtonHeight;

    bool gameOverControls(SDL_Event& event);

    void cycleUp();
    void cycleDown();

public:
    SGameOver();
    ~SGameOver();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();
};


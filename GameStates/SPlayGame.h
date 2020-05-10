#pragma once

#include "GameState.h"
#include "Map.h"
#include "Player.h"
#include "Camera.h"
#include "WTimer.h"
#include <vector>

class SPlayGame : public GameState
{
private:
    Map* m_map;
    Player* m_player;
    Camera m_camera{};
    WTimer m_stepTimer;
    double m_timeAccumulator{ 0.0 };
    double m_updateStep{1.0 / 60.0};

    double m_FPSTime{ 0.0 };
    int m_FPSCount{ 0 };

    void playerControlsKeyHold();
    void playerControlsKeyPress(SDL_Event& event);

    double averageFPS(double timeStep);

    bool m_paused{ false };

public:
    SPlayGame(const char* mapFile = "Assets/Maps/sideScroller.txt", const char* playerFile = "Assets/MrPix.png");
    ~SPlayGame();

    State handleEvents();
    State update();
    void render();

    void pauseGame() { m_paused = true; }
    void resumeGame() { m_paused = false; };
};


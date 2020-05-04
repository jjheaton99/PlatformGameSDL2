#pragma once

#include "GameState.h"
#include "Map.h"
#include "Player.h"
#include "Camera.h"
#include "TimerW.h"
#include <vector>

class SPlayGame : public GameState
{
private:
    Map* m_map;
    Player* m_player;
    Camera m_camera{};
    TimerW m_stepTimer;

public:
    SPlayGame(const char* mapFile = "Assets/Maps/sideScroller.txt", const char* playerFile = "Assets/MrPix.png");
    ~SPlayGame();

    State handleEvents();
    State update();
    void render();
};


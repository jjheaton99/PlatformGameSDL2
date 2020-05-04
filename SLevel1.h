#pragma once

#include "GameState.h"

class SLevel1 : public GameState
{
public:
    SLevel1();
    ~SLevel1();

    void handleEvents();
    void update();
    void render();
};


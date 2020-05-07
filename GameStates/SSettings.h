#pragma once

#include "GameState.h"

class SSettings : public GameState
{
private:

public:
    SSettings();
    ~SSettings();

    State handleEvents();
    State update();
    void render();
};
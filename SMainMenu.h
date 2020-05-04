#pragma once

#include "GameState.h"

class SMainMenu : public GameState
{
public:
    SMainMenu() {};
    ~SMainMenu() {};

    State handleEvents() { return STATE_NULL; };
    State update() { return STATE_NULL; };
    void render() {};
};


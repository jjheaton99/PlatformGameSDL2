#pragma once

#include "Character.h"
#include "Constants.h"
#include <array>
#include <cmath>
#include <cstdint>

class GameObject;

class Player : public Character
{
public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Player();

    virtual void update(double timeStep) override;
    virtual void draw() override;

    static const int spriteSheetCount{ 22 };

private:
    std::array<SDL_Rect, spriteSheetCount> m_spriteRects;
    bool m_isFlipped{ true };

};

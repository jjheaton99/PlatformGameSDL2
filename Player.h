#pragma once

#include "Character.h"
#include "Constants.h"
#include <array>
#include <cmath>
#include <cstdint>

class Player : public Character
{
public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Player();

    void cycleWalkAnimation(double timeStep);
    void cycleIdleAnimation(double timeStep);
    void spriteAnimate(double timeStep);

    virtual void update(double timeStep, std::vector<std::vector<Tile>>& map) override;
    virtual void draw() override;

    static const int spriteSheetCount{ 28 };
    static constexpr double timeBetweenSpriteFrames{ 0.033333 };

private:
    std::array<SDL_Rect, spriteSheetCount> m_spriteRects;
    std::array<SDL_Rect, spriteSheetCount>::size_type m_spriteIndex;
    bool m_isFlipped{ false };
    double m_animationTime{ 0 };
};

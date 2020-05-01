#pragma once

#include "Character.h"
#include "Constants.h"
#include <array>
#include <cmath>
#include <cstdint>

class Player : public Character
{
public:
    Player(const char* fileName, float xStartPos = 0, float yStartPos = 0, float xVel = 0, float yVel = 0);
    ~Player();

    void cycleWalkAnimation(float timeStep);
    void cycleIdleAnimation(float timeStep);
    void spriteAnimate(float timeStep);

    virtual void update(float timeStep, std::vector<std::vector<Tile>>& map) override;
    virtual void draw() override;

    static const int spriteSheetCount{ 28 };
    static constexpr float timeBetweenSpriteFrames{ 0.033333 };

private:
    std::array<SDL_Rect, spriteSheetCount> m_spriteRects;
    std::array<SDL_Rect, spriteSheetCount>::size_type m_spriteIndex;
    bool m_isFlipped{ false };
    float m_animationTime{ 0 };
};

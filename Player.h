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

    void update(double timeStep, std::vector<std::vector<Tile>>& map, Camera& camera);
    void draw(Camera& camera) override;

    static const int spriteSheetCount{ 28 };
    static constexpr double timeBetweenSpriteFrames{ 0.033333 };

private:
    std::array<SDL_Rect, spriteSheetCount> m_spriteRects;
    std::array<SDL_Rect, spriteSheetCount>::size_type m_spriteIndex;
    bool m_isFlipped{ false };
    double m_animationTime{ 0 };
    double m_maxVel{ 5000 };

    void cycleWalkAnimation(double timeStep);
    void cycleIdleAnimation(double timeStep);
    void spriteAnimate(double timeStep);

    void moveCamera(Camera& camera);
};

#pragma once

#include "GroundedCharacter.h"
#include "Constants.h"
#include <array>
#include <cmath>
#include <cstdint>

//player character class
class Player : public GroundedCharacter
{
private:
    static const int m_spriteSheetCount{ 28 };

    std::array<SDL_Rect, m_spriteSheetCount> m_spriteRects;
    std::array<SDL_Rect, m_spriteSheetCount>::size_type m_spriteIndex;
    bool m_isFlipped{ false };

    //member variables for controlling dodge timing and animation
    const double m_dodgeCooldown{0.70};
    bool m_dodgeCooling{ false };
    bool m_dodgingLeft{ false };
    bool m_dodgingRight{ false };
    int m_dodgeStepCount{ 0 };
    double m_angle{ 0.0 };

    int m_animationStep{ 0 };
    void cycleWalkAnimation();
    void cycleIdleAnimation();
    void spriteAnimate();

    void motion() override;
    void moveCamera(Camera& camera);

public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);
    ~Player();

    void update(std::vector<std::vector<Tile>>& map, Camera& camera);
    void cameraDraw(Camera& camera) override;

    bool dodgeCooling() { return m_dodgeCooling; }
    bool isDodging() { return m_dodgingLeft || m_dodgingRight; }
    void dodgeLeft();
    void dodgeRight();
};

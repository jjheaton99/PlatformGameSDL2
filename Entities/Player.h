#pragma once

#include "GroundedCharacter.h"
#include "Constants.h"
#include "PlayerSideAttack.h"
#include <array>
#include <cmath>
#include <cstdint>

//player character class
class Player : public GroundedCharacter
{
private:
    static const int m_spriteSheetCount{ 26 };

    std::array<SDL_Rect, m_spriteSheetCount> m_spriteRects;
    std::array<SDL_Rect, m_spriteSheetCount>::size_type m_spriteIndex;
    bool m_facingLeft{ false };

    PlayerSideAttack m_sideAttack{ 1.0 * m_position.getx() + 50.0, 1.0 * m_position.gety() + 60.0 };

    //member variables for controlling dodge timing and animation
    const double m_dodgeDuration{ 0.3 };
    const double m_dodgeCooldown{ 0.7 };
    bool m_dodgeCooling{ false };
    bool m_dodgingLeft{ false };
    bool m_dodgingRight{ false };
    int m_dodgeStepCount{ 0 };
    double m_angle{ 0.0 };

    int m_crouchStepCount{ 0 };

    int m_animationStep{ 0 };
    void cycleWalkAnimation();
    void cycleIdleAnimation();
    void spriteAnimate();

    void motion() override;
    void moveCamera(Camera& camera);

public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);

    void update(const std::vector<std::vector<Tile>>& map, Camera& camera);
    void cameraDraw(const Camera& camera) const override;

    bool dodgeCooling() const { return m_dodgeCooling; }
    bool isDodging() const { return m_dodgingLeft || m_dodgingRight; }
    void dodgeLeft();
    void dodgeRight();
    void attackLeft();
    void attackRight();
    bool isAttacking() const { return m_sideAttack.isAttacking(); }
    bool isFacingLeft() const { return m_facingLeft; }
};

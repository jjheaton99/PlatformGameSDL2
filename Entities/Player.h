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

    PlayerSideAttack m_sideAttack{};

    //member variables for controlling dodge timing and animation
    const double m_dodgeDuration{ 0.25 };
    const double m_dodgeCooldown{ 0.6 };
    int m_dodgeFrames = static_cast<int>(m_dodgeDuration / Constants::updateStep);
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
    void setCollider() override { m_collider.setPosition(m_position.getx() + 22.0, m_position.gety()); }

public:
    Player(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0);

    void update(const std::vector<std::vector<Tile>>& map, Camera& camera, std::vector<Character*>& enemies) override;
    void cameraDraw(const Camera& camera) const override;

    bool dodgeCooling() const { return m_dodgeCooling; }
    bool isDodging() const { return m_dodgingLeft || m_dodgingRight; }
    void dodgeLeft() { m_dodgingLeft = true; }
    void dodgeRight() { m_dodgingRight = true; }
    void dodgeCancel();

    void attackLeft();
    void attackRight();
    void attackCancel();
    bool isAttacking() const { return m_sideAttack.isAttacking(); }

    bool isFacingLeft() const { return m_facingLeft; }
};

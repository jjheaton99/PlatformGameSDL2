#pragma once

#include "GroundedCharacter.h"
#include "Constants.h"
#include "PlayerSideAttack.h"
#include "PlayerBoomerang.h"
#include <array>
#include <cmath>
#include <cstdint>

class GroundedEnemy;

//player character class
class Player : public std::enable_shared_from_this<Character>, public GroundedCharacter
{
private:
    static const int m_spriteSheetCount{ 26 };

    const int m_maxHP{ 20 };

    std::array<SDL_Rect, m_spriteSheetCount> m_spriteRects;
    std::array<SDL_Rect, m_spriteSheetCount>::size_type m_spriteIndex;
    bool m_facingLeft{ false };

    PlayerSideAttack m_sideAttack{};
    PlayerBoomerang m_boomerang{};
    bool m_throwBoomerang{ false };

    //member variables for controlling dodge timing and animation
    const double m_dodgeDuration{ 0.25 };
    const double m_dodgeCooldown{ 0.6 };
    const int m_dodgeFrames = static_cast<int>(m_dodgeDuration / Constants::updateStep);
    bool m_dodgeCooling{ false };
    bool m_dodgingLeft{ false };
    bool m_dodgingRight{ false };
    const double m_minDodgeVel{ 25.0 };
    int m_dodgeStepCount{ 0 };

    double m_angle{ 0.0 };

    int m_crouchStepCount{ 0 };

    const double m_iDuration{ 0.25 };
    const int m_iFrames{ static_cast<int>(m_iDuration / Constants::updateStep) };
    bool m_invincible{ false };
    int m_iFrameCount{ 0 };

    int m_animationStep{ 0 };
    void cycleWalkAnimation();
    void cycleIdleAnimation();
    void spriteAnimate();

    void motion() override;
    void moveCamera(Camera& camera);
    void setCollider() override;

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Character& player) override {};

public:
    Player(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/MrPix.png", int hitPoints = 15);
    ~Player();

    void update(const std::vector<std::vector<Tile>>& map, Camera& camera, std::vector<std::shared_ptr<Character>>& enemies);
    void cameraDraw(const Camera& camera) const override;

    void makeAirborne() override;

    bool dodgeCooling() const { return m_dodgeCooling; }
    bool isDodging() const { return m_dodgingLeft || m_dodgingRight; }
    void dodgeLeft();
    void dodgeRight();
    void dodgeCancel();

    void attackLeft();
    void attackRight();
    void attackCancel();
    bool isAttacking() const { return m_sideAttack.isAttacking(); }
    void throwBoomerang();
    bool boomerangIsFlying() const { return m_boomerang.isFlying(); }

    bool isFacingLeft() const { return m_facingLeft; }
    
    bool isInvincible() const { return m_invincible; }
    void startiFrames();
};

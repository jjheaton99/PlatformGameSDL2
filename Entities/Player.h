#pragma once

#include "GroundedCharacter.h"
#include "Constants.h"
#include "PlayerSwingAttack.h"
#include "PlayerStabAttack.h"
#include "PlayerDownAttack.h"
//#include "PlayerBoomerang.h"
#include <array>
#include <cmath>
#include <cstdint>

class GroundedEnemy;

//player character class
class Player : public std::enable_shared_from_this<Character>, public GroundedCharacter
{
private:
    PlayerSwingAttack m_swingAttack{};
    PlayerStabAttack m_stabAttack{};
    PlayerDownAttack m_downAttack{};
    //PlayerBoomerang m_boomerang{};
    //bool m_throwBoomerang{ false };

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

    bool m_jumpingHigher{ false };
    int m_jumpHigherCount{ 0 };

    bool m_floatingLeft{ false };
    bool m_floatingRight{ false };
    double m_maxFloatSpeed{ 11.0 };
    double m_floatAccel{ 1.1 };

    double m_wallslideSpeed{ 7.0 };

    int m_crouchStepCount{ 0 };

    const double m_iDuration{ 0.25 };
    const int m_iFrames{ static_cast<int>(m_iDuration / Constants::updateStep) };
    bool m_invincible{ false };
    int m_iFrameCount{ 0 };

    void cycleWalkAnimation();
    void cycleIdleAnimation();
    void animateSprite() override;

    void motion() override;
    void moveCamera(Camera& camera);
    void setCollider() override;

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player) override {};

public:
    Player(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, const char* fileName = "Assets/MrPix.png", int hitPoints = 30, int spriteSheetCount = 28);
    ~Player();

    void update(const std::vector<std::vector<Tile>>& map, Camera& camera, std::vector<std::shared_ptr<Character>>& enemies);
    void cameraDraw(const Camera& camera) const override;

    void makeAirborne() override;

    bool dodgeCooling() const { return m_dodgeCooling; }
    bool isDodging() const { return m_dodgingLeft || m_dodgingRight; }
    void dodgeLeft();
    void dodgeRight();
    void dodgeCancel();
    double getDodgeCooldownFraction() const;

    void swingAttackLeft();
    void swingAttackRight();
    void stabAttackLeft();
    void stabAttackRight();
    void downAttack();
    void attackCancel();
    bool isSwingAttacking() const { return m_swingAttack.isAttacking(); }
    bool isStabAttacking() const { return m_stabAttack.isAttacking(); }
    bool isDownAttacking() const { return m_downAttack.isAttacking(); }
    bool isAttacking() const { return isSwingAttacking() || isStabAttacking() || isDownAttacking(); }
    //void throwBoomerang();
    //bool boomerangIsFlying() const { return m_boomerang.isFlying(); }
    //double getBoomerangCooldownFraction() const { return m_boomerang.getCooldownFraction(); }

    bool isFacingLeft() const { return m_facingLeft; }
    
    void jumpHigher() { m_jumpingHigher = true; }
    void stopJumpingHigher() { m_jumpingHigher = false; }
    bool isJumpingHigher() { return m_jumpingHigher; }

    void floatLeft();
    void floatRight();

    void removeHP(int HP) override;
    bool isInvincible() const { return m_invincible; }
    void startiFrames();
};

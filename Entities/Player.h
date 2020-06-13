#pragma once

#include "GroundedCharacter.h"
#include "Constants.h"
#include "MeleeObject.h"
#include "PlayerSwingAttack.h"
#include "PlayerStabAttack.h"
#include "PlayerDownAttack.h"
#include "PlayerBoomerang.h"
#include <array>
#include <cmath>
#include <cstdint>

class GroundedEnemy;

//player character class
class Player : public std::enable_shared_from_this<Character>, public GroundedCharacter
{
private:
    std::unique_ptr<MeleeObject> m_meleeAttack{ std::make_unique<PlayerSwingAttack>() };
    std::unique_ptr<MeleeObject> m_downAttack{ std::make_unique<PlayerDownAttack>() };
    PlayerBoomerang m_boomerang{};

    //member variables for controlling dodge timing and animation
    const double m_dodgeDuration{ 0.25 };
    const double m_dodgeCooldown{ 0.6 };
    const int m_dodgeFrames = static_cast<int>(m_dodgeDuration / Constants::updateStep);
    bool m_dodgeCooling{ false };
    bool m_dodgingLeft{ false };
    bool m_dodgingRight{ false };
    const double m_minDodgeVel{ 25.0 };
    int m_dodgeStepCount{ 0 };

    bool m_jumpingHigher{ false };
    int m_jumpHigherCount{ 0 };

    bool m_floatingLeft{ false };
    bool m_floatingRight{ false };
    double m_maxFloatSpeed{ 11.0 };
    double m_floatAccel{ 1.1 };

    double m_wallslideSpeed{ 7.0 };

    int m_crouchStepCount{ 0 };
    bool m_drop{ false };

    const double m_iDuration{ 0.25 };
    const int m_iFrames{ static_cast<int>(m_iDuration / Constants::updateStep) };
    bool m_invincible{ false };
    int m_iFrameCount{ 0 };

    bool m_slowDebuff{ false };
    int m_slowDebuffCount{ 0 };

    int m_healthPotions{ 20 };

    void cycleWalkAnimation();
    void cycleIdleAnimation();
    void animateSprite() override;

    void motion() override;
    void moveCamera(Camera& camera);
    void setCollider() override;

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player) override {};

public:
    Player(double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, std::string fileName = "Assets/MrPix.png", int hitPoints = 200, int spriteSheetCount = 28);
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

    void meleeAttackLeft();
    void meleeAttackRight();
    void downAttack();
    void attackCancel();
    bool isAttacking() const { return m_meleeAttack->isAttacking() || m_downAttack->isAttacking(); }

    void throwBoomerangLeft();
    void throwBoomerangRight();
    bool boomerangIsFlying() const { return m_boomerang.isFlying(); }
    double getBoomerangCooldownFraction() const { return m_boomerang.getCooldownFraction(); }

    bool isFacingLeft() const { return m_facingLeft; }
    
    void jumpHigher() { m_jumpingHigher = true; }
    void stopJumpingHigher() { m_jumpingHigher = false; }
    bool isJumpingHigher() { return m_jumpingHigher; }

    bool isCrouched() const { return m_crouched; }
    void dropThroughPlatform() { m_drop = true; }

    void floatLeft();
    void floatRight();

    void removeHP(int HP) override;
    bool isInvincible() const { return m_invincible; }
    void startiFrames();

    void inflictSlow() { m_slowDebuff = true; }

    void drinkHealthPotion();
    int getPotionCount() const { return m_healthPotions; }
};

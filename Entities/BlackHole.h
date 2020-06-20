#pragma once

#include "PlayerRangedAttack.h"

class BlackHole : public PlayerRangedAttack
{
private:
    const int m_spriteSheetCount{ 7 };
    int m_animationStep{ 0 };
    std::vector<SDL_Rect> m_spriteRects;
    std::vector<SDL_Rect>::size_type m_spriteIndex{ 0 };

    const double m_pullSpeed{ 7.0 };
    const double m_speed{ 1.5 };
    const double m_duration{ 5.0 };
    const double m_pullRange{ 1000.0 };

    int m_flyingCount{ 0 };
    int m_implosionCount{ 0 };

    std::vector<std::shared_ptr<Character>> m_pulledEnemies;

    SoundEffect m_flyingSound{ "Assets/Sounds/PlayerSounds/AttackSounds/blackHole.wav" };
    SoundEffect m_boomSound{ "Assets/Sounds/PlayerSounds/AttackSounds/boom.wav" };

    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map) override;
    bool m_collided{ false };
    void pullEnemies(std::vector<std::shared_ptr<Character>>& enemies);
    bool enemyCollideCheck(std::vector<std::shared_ptr<Character>>& enemies) override { return false; };
    void pulledEnemiesCollideCheck();
    void pulledEnemiesExplode();

    void motion() override;
    void setCollider() override { m_collider.setPosition(m_position.getx() + 50.0, m_position.gety() + 50.0); }

public:
    BlackHole();

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::vector<std::shared_ptr<Character>>& enemies, std::shared_ptr<Character> player) override;
};


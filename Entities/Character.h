#pragma once

#include "GameObject.h"
#include "Constants.h"
#include "Tile.h"
#include <cmath>
#include <cstdint>
#include <algorithm>

//base class for characters e.g. players, enemies, npcs
class Character : public GameObject
{
protected:
    bool m_dead{ false };

    Vector2D<double> m_velocity;

    double m_yMaxSpeed{ 0 };
    double m_xMaxSpeed{ 0 };

    std::vector<SDL_Rect> m_solidHitBoxes;
    std::vector<SDL_Rect> m_platformHitBoxes;
    bool m_collidingWithLadder{ false };
    int m_ladderxPos{ 0 };

    //for checking collisions with map tiles
    virtual void mapCollideCheck(const std::vector<std::vector<Tile>>& map) = 0;
    virtual void collideTileHitBoxes(const std::vector<std::vector<Tile>>& map, int characterRow,
        int characterColumn, int tileSize, const SDL_Rect& characterColliderBox);
    //for checking collisions with the edge of the map
    virtual bool edgeCheck(const Camera& camera) = 0;

    //for adjusting character velocity
    virtual void motion() = 0;

public:
    Character(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0);
    virtual ~Character();

    bool isDead() const { return m_dead; }

    virtual void update(const std::vector<std::vector<Tile>>& map, Camera& camera) = 0;

    const Vector2D<double>& getVel() const { return m_velocity; }
    void setVel(double xVel, double yVel) { m_velocity = Vector2D<double>{ xVel, yVel }; }
    void setDirection(double angle) { m_velocity.rotate(angle); }
    int getLadderxPos() const { return m_ladderxPos; }
    bool collidingWithLadder() const { return m_collidingWithLadder; }
};


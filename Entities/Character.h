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
    //characters have a velocity for movement
    Vector2D<double> m_velocity;

    //for checking collisions with map tiles
    virtual void mapCollideCheck(std::vector<std::vector<Tile>>& map) = 0;
    virtual std::vector<SDL_Rect> getCollideTileBoxes(std::vector<std::vector<Tile>>& map, int characterRow, 
        int characterColumn, int tileSize, SDL_Rect& characterCollider);
    //for checking collisions with the edge of the map
    virtual void edgeCheck(Camera& camera) = 0;

    //for adjusting character velocity
    virtual void motion(double acceleration, double deceleration, double maxVel) = 0;

public:
    Character(const char* fileName, double xStartPos = 0, double yStartPos = 0, double xVel = 0, double yVel = 0, int colliderWidth = 0, int colliderHeight = 0);
    ~Character();

    const Vector2D<double>& getVel() { return m_velocity; }
    void setVel(double xVel, double yVel) { m_velocity = Vector2D<double>{ xVel, yVel }; }
    void setDirection(double angle) { m_velocity.rotate(angle); }
};


#include "GroundedEnemy.h"

GroundedEnemy::GroundedEnemy(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel)
    : GroundedCharacter(fileName, xStartPos, yStartPos, xVel, yVel, 100, 100)
{
    m_yMaxSpeed = 30.0;
    m_xMaxSpeed = 12.0;
    m_walkAcceleration = 0.8;

    m_dstRect.w = 100;
    m_dstRect.h = 100;
}

void GroundedEnemy::update(const std::vector<std::vector<Tile>>& map, Camera& camera)
{}

void GroundedEnemy::enemyControls()
{}

void GroundedEnemy::motion()
{}

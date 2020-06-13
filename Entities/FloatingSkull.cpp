#include "FloatingSkull.h"
#include "Player.h"

FloatingSkull::FloatingSkull(double xStartPos, double yStartPos, double xVel, double yVel, std::string fileName, double colliderWidth, double colliderHeight, int hitPoints, int spriteSheetCount, int damage)
    : FlyingEnemy(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints, spriteSheetCount, damage)
{
    m_spriteRects.push_back({ 0, 0, 40, 40 });
    m_spriteRects.push_back({ 42, 0, 40, 40 });
    m_spriteRects.push_back({ 0, 42, 40, 40, });
    m_spriteRects.push_back({ 42, 42, 40, 40 });
    m_spriteRects.push_back({ 84, 0, 40, 40 });
    m_spriteRects.push_back({ 0, 84, 40, 40 });
    m_spriteRects.push_back({ 42, 84, 40, 40 });
    m_spriteRects.push_back({ 84, 42, 40, 40 });
    m_spriteRects.push_back({ 84, 84, 40, 40 });
    m_spriteRects.push_back({ 126, 0, 40, 40 });
    m_spriteRects.push_back({ 0, 126, 40, 40 });
    m_spriteRects.push_back({ 42, 126, 40, 40 });
    m_spriteRects.push_back({ 126, 42, 40, 40 });
    m_spriteRects.push_back({ 84, 126, 40, 40 });
    m_spriteRects.push_back({ 126, 84, 40, 40 });
    m_spriteRects.push_back({ 126, 126, 40, 40 });

    m_dstRect.w = 140;
    m_dstRect.h = 140;

    m_texture.setBlendingMode(SDL_BLENDMODE_BLEND);
    m_texture.setAlpha(120);
}

void FloatingSkull::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, std::shared_ptr<Character> player)
{
    if (m_hitPoints <= 0)
    {
        kill();
    }

    //only update when within certain distance from camera for performance
    if (m_position.getx() > 1.0 * camera.getx() - m_updateRange && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + m_updateRange
        && m_position.gety() > 1.0 * camera.gety() - m_updateRange && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + m_updateRange)
    {
        enemyControls(player);

        //edge check goes before map collision check to prevent vector subcript error when going off the edge
        if (edgeCheck(camera))
        {
            //collider position is moved after each function that can change character position
            setCollider();
        }

        bool collided{ sweepMapCollideCheck(map) };
        if (collided)
        {
            setCollider();
        }

        if (!collided)
        {
            m_position.add(m_velocity);
            setCollider();
        }

        m_dstRect.x = static_cast<int>(m_position.getx());
        m_dstRect.y = static_cast<int>(m_position.gety());

        animateSprite();
        cycleDamageFlash();

        if (m_projectile == ProjectileType::SKULL)
        {
            m_projectile = ProjectileType::NONE;
        }
        if (++m_projectileCount > 150)
        {
            m_projectileCount = 0;
            m_projectile = ProjectileType::SKULL;
        }
    }
}

void FloatingSkull::enemyControls(const std::shared_ptr<Character> player)
{
    Vector2D<double> relPos{ player->getPos() + Vector2D<double>{50, 50} - m_position };
    double scaleFactor{ m_acceleration / relPos.magnitude() };

    if (relPos.magnitude() > 600.0)
    {
        m_velocity.add(scaleFactor * relPos);
    }
    else if (relPos.magnitude() < 450.0)
    {
        m_velocity.subtract(scaleFactor * relPos);
    }
    else
    {
        if (MTRandom::getRandomInt(0, 99) < 1)
        {
            setVel(m_maxSpeed / 2.0, 0.0);
            m_velocity.rotate(MTRandom::getRandomDouble(0.0, 360.0));
        }
        m_velocity.scale(0.99);
    }

    double velMag{ m_velocity.magnitude() };

    //remove excess speed
    if (velMag > m_maxSpeed)
    {
        scaleFactor = (velMag - m_maxSpeed) / velMag;
        m_velocity.subtract(scaleFactor * m_velocity);
    }

    if (m_velocity.getx() > 0)
    {
        m_facingLeft = false;
    }
    else
    {
        m_facingLeft = true;
    }
}

void FloatingSkull::animateSprite()
{
    ++m_animationStep;
    if (m_animationStep >= 6)
    {
        m_animationStep = 0;
        ++m_spriteIndex;
        if (m_spriteIndex > 15)
        {
            m_spriteIndex = 0;
        }
    }

    m_srcRect = m_spriteRects[m_spriteIndex];
}
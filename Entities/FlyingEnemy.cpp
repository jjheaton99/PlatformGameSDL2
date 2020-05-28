#include "FlyingEnemy.h"
#include "Player.h"

FlyingEnemy::FlyingEnemy(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight, int hitPoints, int damage)
    : FlyingCharacter(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight, hitPoints), m_damage{ damage }
{}

void FlyingEnemy::update(const std::vector<std::vector<Tile>>& map, const Camera& camera, Character& player)
{
    if (m_hitPoints <= 0)
    {
        kill();
    }

    //only update when within certain distance from camera for performance
    if (m_position.getx() > 1.0 * camera.getx() - 1000.0 && m_position.getx() < 1.0 * camera.getx() + 1.0 * camera.getw() + 1000.0
        && m_position.gety() > 1.0 * camera.gety() - 1000.0 && m_position.gety() < 1.0 * camera.gety() + 1.0 * camera.geth() + 1000.0)
    {
        enemyControls(player);

        //edge check goes before map collision check to prevent vector subcript error when going off the edge
        if (edgeCheck(camera))
        {
            //collider position is moved after each function that can change character position
            setCollider();
        }

        bool collided{ sweepMapCollideCheck(map) || attackPlayer(player) };
        if (collided)
        {
            setCollider();
        }

        //std::cout << m_velocity.gety() << "   " << m_velocity.getx() << '\n';
        //std::cout << m_position.gety() << "   " << m_position.getx() << '\n';
        //std::cout << m_collider.getHitBox().y << "   " << m_collider.getHitBox().x << '\n';

        if (!collided)
        {
            m_position.add(m_velocity);
            setCollider();
        }

        m_dstRect.x = static_cast<int>(m_position.getx());
        m_dstRect.y = static_cast<int>(m_position.gety());
    }
}

bool FlyingEnemy::attackPlayer(Character& player)
{
    if (!dynamic_cast<Player&>(player).isInvincible() && (m_velocity.magnitude() < (m_position - player.getPos()).magnitude()))
    {
        Collider::sweptObstacleTuple sweptCollider{ player.getCollider(), Collider::xOverlap(m_collider, player.getCollider()), Collider::yOverlap(m_collider, player.getCollider()) };
        auto collideResult{ m_collider.sweptAABBCheck(m_velocity, player.getVel(), sweptCollider) };
        double deflectionFactor{ 1.0 };
        bool collided{ false };
        //double recoil{ 10.0 };

        switch (collideResult.first)
        {
        case Collider::TOP:
            m_velocity.yScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{0.0, -recoil});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::BOTTOM:
            m_velocity.yScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{0.0, recoil});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::LEFT:
            m_velocity.xScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{-recoil, 0.0});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::RIGHT:
            m_velocity.xScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{recoil, 0.0});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::OVERLAP_TOP:
            m_velocity.yScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{0.0, -recoil});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::OVERLAP_BOTTOM:
            m_velocity.yScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{0.0, recoil});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::OVERLAP_LEFT:
            m_velocity.xScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{-recoil, 0.0});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::OVERLAP_RIGHT:
            m_velocity.xScale(-deflectionFactor);
            //m_velocity.add(Vector2D<double>{recoil, 0.0});
            m_position.add(m_velocity);
            collided = true;
            break;

        case Collider::NONE:
        default:
            break;
        }

        if (collided)
        {
            player.removeHP(m_damage);
            dynamic_cast<Player&>(player).startiFrames();
            return true;
        }
    }
    return false;
}

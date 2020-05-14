#include "GroundedCharacter.h"

GroundedCharacter::GroundedCharacter(const char* fileName, double xStartPos, double yStartPos, double xVel, double yVel, int colliderWidth, int colliderHeight)
    : Character(fileName, xStartPos, yStartPos, xVel, yVel, colliderWidth, colliderHeight)
{}

GroundedCharacter::~GroundedCharacter()
{}

//resets velocity if colliding with edge of map
bool GroundedCharacter::edgeCheck(const Camera& camera)
{
    if (m_collider.getHitBox().x <= 0)
    {
        m_position.subtract(1.0 * m_collider.getHitBox().x - 1.0, 0);
        setVel(0, m_velocity.gety());
        return true;
    }
    else if (m_collider.getHitBox().x >= camera.getxBoundary() - m_collider.getHitBox().w)
    {
        m_position.subtract(1.0 * m_collider.getHitBox().x - 1.0 * camera.getxBoundary() + 1.0 * m_collider.getHitBox().w + 1.0, 0);
        setVel(0, m_velocity.gety());
        return true;
    }
    else if (m_collider.getHitBox().y <= 0)
    {
        m_position.subtract(0, 1.0 * m_collider.getHitBox().y - 1.0);
        setVel(m_velocity.getx(), 0);
        return true;
    }
    else if (m_collider.getHitBox().y >= camera.getyBoundary() - m_collider.getHitBox().h)
    {
        if (m_movement == AIRBORNE)
        {
            m_movement = STOP;
        }
        m_position.subtract(0, 1.0 * m_collider.getHitBox().y - 1.0 * camera.getyBoundary() + 1.0 * m_collider.getHitBox().h + 1.0);
        setVel(m_velocity.getx(), 0);
        return true;
    }
    return false;
}

//checks if character is standing on a solid map tile
bool GroundedCharacter::checkForGround(const std::vector<std::vector<Tile>>& map, int characterRow,
    int characterColumn, int tileSize, const SDL_Rect& characterColliderBox)
{
    //allows characters to continue climbing when in contact with ladders
    if (m_collidingWithLadder)
    {
        return true;
    }

    for (int row{ characterRow }; row * tileSize <= characterColliderBox.y + characterColliderBox.h; ++row)
    {
        for (int column{ characterColumn }; column * tileSize <= characterColliderBox.x + characterColliderBox.w; ++column)
        {
            if ((map[row][column].getType() == Tile::SOLID && !m_crouched) || map[row][column].getType() == Tile::PLATFORM)
            {
                //Checks for blocks directly below character whilst not colliding
                if ((m_movement != AIRBORNE)
                    && (map[row][column].getCollider().getHitBox().x > (characterColliderBox.x - map[row][column].getCollider().getHitBox().w))
                    && (map[row][column].getCollider().getHitBox().x < (characterColliderBox.x + characterColliderBox.w)))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

//adjustments for when character lands on top of a block
void GroundedCharacter::deflectUp(double overlap)
{
    m_position.subtract(0, overlap);
    //puts character into walking mode when landing on something
    m_movement = STOP;
    m_velocity.yScale(0);
}

//jumping into overhead block
void GroundedCharacter::deflectDown(double overlap)
{
    m_position.add(0, overlap);
    m_velocity.yScale(0);
}

//colliding into the left of a block
void GroundedCharacter::deflectLeft(double overlap)
{
    m_position.subtract(overlap, 0);
    m_velocity.xScale(0);
}

//colliding into right of a block
void GroundedCharacter::deflectRight(double overlap)
{
    m_position.add(overlap, 0);
    m_velocity.xScale(0);
}

//checks for collision with solid map tiles and adjusts position/velocity
//collision handling is based on the overlap between colliders
void GroundedCharacter::mapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    int tileSize{ map[0][0].getSize() };
    SDL_Rect characterColliderBox{ m_collider.getHitBox() };
    //character column and row variables are the position of the character in terms of map tiles
    int characterColumn{ (characterColliderBox.x - (characterColliderBox.x % tileSize)) / tileSize };
    int characterRow{ (characterColliderBox.y - (characterColliderBox.y % tileSize)) / tileSize };

    collideTileHitBoxes(map, characterRow, characterColumn, tileSize, characterColliderBox);

    //for collisions with platform tiles only
    if (m_solidHitBoxes.size() == 0 && !m_crouched && !m_hasCrouched)
    {
        for (const auto& box : m_platformHitBoxes)
        {
            //overlap variables are the overlap of the character collider with tile collider in x or y direction
            double xOverlap{ Collider::axisBoxOverlap(characterColliderBox.x, box.x, characterColliderBox.w, box.w) };
            double yOverlap{ Collider::axisBoxOverlap(characterColliderBox.y, box.y, characterColliderBox.h, box.h) };

            //platforms only care about collisions from above
            if ((yOverlap < xOverlap) && (characterColliderBox.y < box.y) && m_velocity.gety() > 0.0)
            {
                deflectUp(yOverlap);
            }
        }
    }

    int currentIndex{ 0 };
    double xMaxOverlap{ 0 };
    double yMaxOverlap{ 0 };
    double maxOverlap{ 0 };
    int maxOverlapIndex{ 0 };
    //loops over the solid tiles selects the one with greatest overlap (can be x or y) with character collider
    for (const auto& box : m_solidHitBoxes)
    {
        double xOverlap{Collider::axisBoxOverlap(characterColliderBox.x, box.x, characterColliderBox.w, box.w)};
        double yOverlap{ Collider::axisBoxOverlap(characterColliderBox.y, box.y, characterColliderBox.h, box.h) };
        
        if (xOverlap > xMaxOverlap)
        {
            xMaxOverlap = xOverlap;
            if (xOverlap > maxOverlap)
            {
                maxOverlap = xOverlap;
                maxOverlapIndex = currentIndex;
            }
        }
        if (yOverlap > yMaxOverlap)
        {
            yMaxOverlap = yOverlap;
            if (yOverlap > maxOverlap)
            {
                maxOverlap = yOverlap;
                maxOverlapIndex = currentIndex;
            }
        }

        ++currentIndex;
    }

    //if the character collides with a small amount of the tile, the overlap algorithm could wrongly choose the collision direction
    //therefore the collision handling is based on only the tile with max overlap
    //however doesnt help for single tile corner collisions
    if ((yMaxOverlap < xMaxOverlap) && (characterColliderBox.y < m_solidHitBoxes[maxOverlapIndex].y))
    {
        deflectUp(yMaxOverlap);
    }
    else if ((yMaxOverlap < xMaxOverlap) && (characterColliderBox.y > m_solidHitBoxes[maxOverlapIndex].y))
    {
        deflectDown(yMaxOverlap);
    }
    else if ((xMaxOverlap < yMaxOverlap) && (characterColliderBox.x < m_solidHitBoxes[maxOverlapIndex].x))
    {
        deflectLeft(xMaxOverlap);
    }
    else if ((xMaxOverlap < yMaxOverlap) && (characterColliderBox.x > m_solidHitBoxes[maxOverlapIndex].x))
    {
        deflectRight(xMaxOverlap);
    }

    //causes character to fall when stepping off platform or solid tile
    if (!checkForGround(map, characterRow, characterColumn, tileSize, characterColliderBox))
    {
        m_movement = AIRBORNE;
    }
}

void GroundedCharacter::crouch() 
{ 
    m_crouched = true;
    m_hasCrouched = true;
}
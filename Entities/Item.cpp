#include "Item.h"

Item::Item(ItemType type, bool shopItem, double xPos, double yPos)
    : GameObject(0.0, 0.0, 0.0, 0.0, 50, 50), m_type{type}, m_shopItem{shopItem}
{
    switch (type)
    {
    case GameObject::ItemType::POTION:
        m_texture.load("Assets/Items/healthPotion.png");
        m_srcRect = { 0, 0, 16, 19 };
        m_dstRect.w = 50;
        m_dstRect.h = 59;
        m_price = 100;
        break;

    case GameObject::ItemType::SWORD:
        m_texture.load("Assets/Items/sword.png");
        m_srcRect = { 0, 0, 12, 44 };
        m_dstRect.w = 37;
        m_dstRect.h = 137;
        m_price = 1000;
        break;

    case GameObject::ItemType::AXE:
        m_texture.load("Assets/Items/axe.png");
        m_srcRect = { 0, 0, 26, 57 };
        m_dstRect.w = 81;
        m_dstRect.h = 178;
        m_price = 5000;
        break;

    case GameObject::ItemType::BOOMERANG:
        m_texture.load("Assets/Items/boomerang.png");
        m_srcRect = { 0, 0, 34, 34 };
        m_dstRect.w = 106;
        m_dstRect.h = 106;
        m_price = 4000;
        break;

    case GameObject::ItemType::DOWN_AXE:
        m_texture.load("Assets/Items/downAxe.png");
        m_srcRect = { 0, 0, 22, 23 };
        m_dstRect.w = 69;
        m_dstRect.h = 72;
        m_price = 3000;
        break;

    case GameObject::ItemType::NONE:
    default:
        break;
    }

    setPos(xPos - ((m_dstRect.w - 50.0) / 2.0), yPos - m_dstRect.h + 50.0);

    if (m_shopItem)
    {
        m_priceTexture.loadText(std::to_string(m_price), { 255, 255, 255 });
    }
}

void Item::update(const std::vector<std::vector<Tile>>& map, const Camera& camera)
{
    if (!m_landed && !m_shopItem)
    {
        bool collided{ sweepMapCollideCheck(map) };

        if (!collided)
        {
            m_position.add(m_velocity);
            setCollider();
        }

        m_dstRect.x = static_cast<int>(m_position.getx());
        m_dstRect.y = static_cast<int>(m_position.gety());

        motion();
    }
    else
    {
        setCollider();

        m_dstRect.x = static_cast<int>(m_position.getx());
        m_dstRect.y = static_cast<int>(m_position.gety());

        if (m_shopItem)
        {
            m_priceTexture.setDstRect(m_dstRect.x - (((3 * m_priceTexture.getTextDimensions().x) - m_dstRect.w) / 2) - camera.getx(), 
                m_dstRect.y + m_dstRect.h + 5 - camera.gety(), 3 * m_priceTexture.getTextDimensions().x, 3 * m_priceTexture.getTextDimensions().y);
        }
    }
}

void Item::motion()
{
    m_velocity.add(0.0, Constants::g);
}

void Item::setPriceColour(int playerMoney)
{
    if (playerMoney >= m_price)
    {
        m_priceTexture.setColour(0, 255, 0);
    }
    else
    {
        m_priceTexture.setColour(255, 0, 0);
    }
}

void Item::getCollideTiles(const std::vector<std::vector<Tile>>& map, int itemRow, int itemColumn)
{
    //delete any existing hitboxes 
    m_solidColliders.clear();
    m_platformColliders.clear();

    int startRow{ itemRow - static_cast<int>(m_velocity.magnitude() / Constants::tileSize) - 2 };
    int endRow{ itemRow + static_cast<int>(m_velocity.magnitude() / Constants::tileSize) + static_cast<int>(m_collider.getHitBox().h / Constants::tileSize) + 2 };
    int startColumn{ itemColumn - static_cast<int>(m_velocity.magnitude() / Constants::tileSize) - 2 };
    int endColumn{ itemColumn + static_cast<int>(m_velocity.magnitude() / Constants::tileSize) + static_cast<int>(m_collider.getHitBox().w / Constants::tileSize) + 2 };

    //check rows and cols within map
    if (startRow < 0)
    {
        startRow = 0;
    }
    if (endRow > static_cast<int>(map.size()) - 1)
    {
        endRow = map.size() - 1;
    }
    if (startColumn < 0)
    {
        startColumn = 0;
    }
    if (endColumn > static_cast<int>(map[0].size()) - 1)
    {
        endColumn = map[0].size() - 1;
    }

    for (int row{ startRow }; row <= endRow; ++row)
    {
        for (int column{ startColumn }; column <= endColumn; ++column)
        {
            switch (map[row][column].getType())
            {
            case Tile::SOLID:
                m_solidColliders.push_back({
                    map[row][column].getCollider(),
                    Collider::xOverlap(m_collider, map[row][column].getCollider()),
                    Collider::yOverlap(m_collider, map[row][column].getCollider()),
                    });
                break;

            case Tile::PLATFORM:
                m_platformColliders.push_back({
                    map[row][column].getCollider(),
                    Collider::xOverlap(m_collider, map[row][column].getCollider()),
                    Collider::yOverlap(m_collider, map[row][column].getCollider()),
                    });
                break;

            case Tile::BACKGROUND:
            default:
                break;
            }
        }
    }
}

bool Item::sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map)
{
    //character column and row variables are the position of the character in terms of map tiles
    int itemColumn{ Tile::coordToMapIndex(m_collider.getHitBox().x) };
    int itemRow{ Tile::coordToMapIndex(m_collider.getHitBox().y) };

    getCollideTiles(map, itemRow, itemColumn);
    //sort by greatest overlaps
    std::sort(m_solidColliders.begin(), m_solidColliders.end(), [&](const auto& a, const auto& b) {
        return ((std::get<1>(a) > std::get<1>(b) && std::get<1>(a) > std::get<2>(b)) || (std::get<2>(a) > std::get<1>(b) && std::get<2>(a) > std::get<2>(b)));
        }
    );

    bool xCollision{ false };
    bool yCollision{ false };

    Vector2D<double> tempVel{ m_velocity };

    for (auto& sweptCollider : m_solidColliders)
    {
        //if we have had collision in both directions, skip
        if (!xCollision || !yCollision)
        {
            auto result(m_collider.sweptAABBCheck(m_velocity, Vector2D<double>{0.0, 0.0}, sweptCollider));

            switch (result.first)
            {
            case Collider::TOP:
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(0);
                    yCollision = true;
                    m_landed = true;
                }
                break;

            case Collider::BOTTOM:
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(0);
                    yCollision = true;
                }
                break;

            case Collider::LEFT:
            case Collider::RIGHT:
                if (!xCollision)
                {
                    tempVel.xScale(result.second);
                    m_velocity.xScale(0);
                    xCollision = true;
                }
                break;

            case Collider::NONE:
            default:
                break;
            }
        }
    }

    for (auto& sweptCollider : m_platformColliders)
    {
        if (!yCollision)
        {
            auto result{ m_collider.sweptAABBCheck(m_velocity, Vector2D<double>{0.0, 0.0}, sweptCollider) };

            if (result.first == Collider::TOP)
            {
                if (!yCollision)
                {
                    tempVel.yScale(result.second);
                    m_velocity.yScale(0);
                    yCollision = true;
                    m_landed = true;
                }
            }
        }
    }

    if (xCollision || yCollision)
    {
        m_position.add(tempVel);
        setCollider();
    }

    return xCollision || yCollision;
}

void Item::cameraDraw(const Camera& camera) const
{
    //objects off the screen are not rendered
    if (m_collider.collideCheck(camera.getCollider()))
    {
        SDL_Rect relativeDstRect{ m_dstRect.x - camera.getx(), m_dstRect.y - camera.gety(), m_dstRect.w, m_dstRect.h };
        m_texture.draw(m_srcRect, relativeDstRect, m_angle, nullptr, SDL_FLIP_NONE);

        m_priceTexture.draw();
    }
}
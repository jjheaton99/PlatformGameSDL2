#pragma once

#include "GameObject.h"
#include "Tile.h"

class Item : public GameObject
{
protected:
    ItemType m_type;

    bool m_shopItem;
    int m_price;
    TextureObject m_priceTexture{};

    std::vector<Collider::sweptObstacleTuple> m_solidColliders;
    std::vector<Collider::sweptObstacleTuple> m_platformColliders;

    bool m_landed{ false };

    void motion();

    void getCollideTiles(const std::vector<std::vector<Tile>>& map, int itemRow, int itemColumn);
    bool sweepMapCollideCheck(const std::vector<std::vector<Tile>>& map);

    void setCollider() override { m_collider.setPosition(m_position.getx() + ((m_dstRect.w - 50.0) / 2.0), m_position.gety() + m_dstRect.h - 50.0); }

public:
    Item(ItemType type, bool shopItem, double xPos = 0, double yPos = 0);

    void update(const std::vector<std::vector<Tile>>& map, const Camera& camera);
    void cameraDraw(const Camera& camera) const override;

    ItemType getType() const { return m_type; }

    bool isShopItem() const { return m_shopItem; }
    int getPrice() const { return m_price; }
    void setPriceColour(int playerMoney);
};


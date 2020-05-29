#include "UserInterface.h"

UserInterface::UserInterface(std::shared_ptr<Player> player)
    : m_player{player}
{
    updateHearts();
}

UserInterface::~UserInterface()
{
    m_hearts.destroy();
}

void UserInterface::updateHearts()
{
    if (m_player->getHP() <= 0)
    {
        m_hearts.setSrcRect(0, 0, 0, 0);
        m_hearts.setDstRect(20, 20, 0, 0);
    }
    else
    {
        m_hearts.setSrcRect(0, 0, 32 * m_player->getHP(), 32);
        m_hearts.setDstRect(20, 20, 35 * m_player->getHP(), 35);
    }
}

void UserInterface::update()
{
    updateHearts();
}

void UserInterface::draw()
{
    m_hearts.draw();
}
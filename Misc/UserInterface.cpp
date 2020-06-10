#include "UserInterface.h"

UserInterface::UserInterface(std::shared_ptr<Player> player)
    : m_player{player}
{
    updateHearts();

    m_boomerangCooldown.alphaBlendOn();
    m_boomerangCooldown.setSrcRect(0, 0, 32, 32);

    m_dodgeCooldown.alphaBlendOn();
    m_dodgeCooldown.setSrcRect(0, 0, 32, 32);
}

UserInterface::~UserInterface()
{
    m_hearts.destroy();
    m_boomerangCooldown.destroy();
    m_dodgeCooldown.destroy();
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

    m_boomerangCooldown.setDstRect(100, g_screenHeight - 80, 60, 60);
    m_dodgeCooldown.setDstRect(20, g_screenHeight - 80, 60, 60);
}

void UserInterface::draw()
{
    m_hearts.draw();

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 200);

    SDL_Rect boomerangCoolDownCover{ 102, g_screenHeight - 78, 56, static_cast<int>(m_player->getBoomerangCooldownFraction() * 56.0) };
    m_boomerangCooldown.draw();
    SDL_RenderFillRect(g_renderer, &boomerangCoolDownCover);

    SDL_Rect dodgeCoolDownCover{ 22, g_screenHeight - 78, 56, static_cast<int>(m_player->getDodgeCooldownFraction() * 56.0) };
    m_dodgeCooldown.draw();
    SDL_RenderFillRect(g_renderer, &dodgeCoolDownCover);

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_NONE);
}
#include "UserInterface.h"

UserInterface::UserInterface(std::shared_ptr<Player> player)
    : m_player{player}
{
    updateHealthBar();

    m_healthPotion.setSrcRect(0, 0, 32, 32);

    m_boomerangCooldown.alphaBlendOn();
    m_boomerangCooldown.setSrcRect(0, 0, 32, 32);

    m_dodgeCooldown.alphaBlendOn();
    m_dodgeCooldown.setSrcRect(0, 0, 32, 32);
}

UserInterface::~UserInterface()
{
    m_healthBar.destroy();
    m_healthPotion.destroy();
    m_boomerangCooldown.destroy();
    m_dodgeCooldown.destroy();
}

void UserInterface::updateHealthBar()
{
    if (m_player->getHP() <= 0)
    {
        m_healthBarDstRect.w = 0;
    }
    else
    {
        m_healthBarDstRect.w = static_cast<int>((1.0 * m_player->getHP() / m_player->getMaxHP()) * 345);
    }
    m_healthBarBackgroundDstRect.y = g_screenHeight - 80;
    m_healthBarDstRect.y = g_screenHeight - 72;
}

void UserInterface::drawHealthBar()
{
    m_healthBar.setSrcRect(m_healthBarBackgroundSrcRect);
    m_healthBar.setDstRect(m_healthBarBackgroundDstRect);
    m_healthBar.draw();

    m_healthBar.setSrcRect(m_healthBarSrcRect);
    m_healthBar.setDstRect(m_healthBarDstRect);
    m_healthBar.draw();
}

void UserInterface::update()
{
    updateHealthBar();

    m_healthPotion.setDstRect(m_healthBarBackgroundDstRect.x + m_healthBarBackgroundDstRect.w + 20,
        g_screenHeight - 80, 60, 60);
    m_boomerangCooldown.setDstRect(g_screenWidth - 80, g_screenHeight - 80, 60, 60);
    m_dodgeCooldown.setDstRect(g_screenWidth - 160, g_screenHeight - 80, 60, 60);
}

void UserInterface::draw()
{
    drawHealthBar();
    m_healthPotion.draw();

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 200);

    SDL_Rect boomerangCoolDownCover{ g_screenWidth - 78, g_screenHeight - 78, 56, static_cast<int>(m_player->getBoomerangCooldownFraction() * 56.0) };
    m_boomerangCooldown.draw();
    SDL_RenderFillRect(g_renderer, &boomerangCoolDownCover);

    SDL_Rect dodgeCoolDownCover{ g_screenWidth - 158, g_screenHeight - 78, 56, static_cast<int>(m_player->getDodgeCooldownFraction() * 56.0) };
    m_dodgeCooldown.draw();
    SDL_RenderFillRect(g_renderer, &dodgeCoolDownCover);

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_NONE);
}
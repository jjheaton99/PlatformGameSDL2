#include "UserInterface.h"

UserInterface::UserInterface(std::shared_ptr<Player> player)
    : m_player{player}
{
    m_playerHealthValue = player->getHP();
    m_healthBarValues.loadText(std::to_string(m_playerHealthValue) + " / " + std::to_string(player->getMaxHP()), m_textColour);
    updateHealthBar();

    m_healthPotion.setSrcRect(0, 0, 32, 32);
    m_playerPotionCount = player->getPotionCount();
    m_potionCount.loadText(std::to_string(m_playerPotionCount), m_textColour);

    m_rangedCooldown.alphaBlendOn();
    m_rangedCooldown.setSrcRect(0, 0, 32, 32);

    m_dodgeCooldown.alphaBlendOn();
    m_dodgeCooldown.setSrcRect(0, 0, 32, 32);

    m_moneyIcon.setSrcRect(0, 0, 3, 3);
    m_playerMoney = player->getMoney();
    m_moneyCount.loadText(std::to_string(m_playerMoney), {255, 255, 255});
}

UserInterface::~UserInterface()
{}

void UserInterface::updateHealthBar()
{
    if (m_playerHealthValue != m_player->getHP())
    {
        m_playerHealthValue = m_player->getHP();

        if (m_playerHealthValue <= 0)
        {
            m_healthBarDstRect.w = 0;
        }
        else
        {
            m_healthBarDstRect.w = static_cast<int>((1.0 * m_playerHealthValue / m_player->getMaxHP()) * 345);
        }

        m_healthBarValues.loadText(std::to_string(m_playerHealthValue) + " / " + std::to_string(m_player->getMaxHP()), m_textColour);
    }
    m_healthBarBackgroundDstRect.y = g_screenHeight - 80;
    m_healthBarDstRect.y = g_screenHeight - 72;
    m_healthBarValues.setDstRect(90, g_screenHeight - 59, 2 * m_healthBarValues.getTextDimensions().x, 2 * m_healthBarValues.getTextDimensions().y);
}

void UserInterface::drawHealthBar()
{
    m_healthBar.setSrcRect(m_healthBarBackgroundSrcRect);
    m_healthBar.setDstRect(m_healthBarBackgroundDstRect);
    m_healthBar.draw();

    m_healthBar.setSrcRect(m_healthBarSrcRect);
    m_healthBar.setDstRect(m_healthBarDstRect);
    m_healthBar.draw();

    m_healthBarValues.draw();
}

void UserInterface::update()
{
    updateHealthBar();

    m_healthPotion.setDstRect(m_healthBarBackgroundDstRect.x + m_healthBarBackgroundDstRect.w + 20,
        g_screenHeight - 80, 60, 60);
    int playerPotionCount{ m_player->getPotionCount() };
    if (m_playerPotionCount != playerPotionCount)
    {
        m_playerPotionCount = playerPotionCount;
        m_potionCount.loadText(std::to_string(m_playerPotionCount), m_textColour);
    }
    m_potionCount.setDstRect(m_healthBarBackgroundDstRect.x + m_healthBarBackgroundDstRect.w + 28,
        g_screenHeight - 72, 2 * m_potionCount.getTextDimensions().x, 2 * m_potionCount.getTextDimensions().y);

    m_rangedCooldown.setDstRect(g_screenWidth - 80, g_screenHeight - 80, 60, 60);
    GameObject::ItemType rangedType{ m_player->getRangedAttackType() };
    if (m_playerCurrentRangedAttack != rangedType)
    {
        m_playerCurrentRangedAttack = rangedType;
        switch (m_playerCurrentRangedAttack)
        {
        case GameObject::ItemType::NONE:
            m_rangedCooldown.loadPng("Assets/UITextures/empty.png");
        case GameObject::ItemType::BOOMERANG:
            m_rangedCooldown.loadPng("Assets/UITextures/boomerang.png");
        default:
            break;
        }
    }

    m_dodgeCooldown.setDstRect(g_screenWidth - 160, g_screenHeight - 80, 60, 60);

    m_moneyIcon.setDstRect(20, 20, 30, 30);
    int playerMoney{ m_player->getMoney() };
    if (m_playerMoney != playerMoney)
    {
        m_playerMoney = playerMoney;
        m_moneyCount.loadText(std::to_string(m_playerMoney), { 255, 255, 255 });
    }
    m_moneyCount.setDstRect(70, 22, 3 * m_moneyCount.getTextDimensions().x, 3 * m_moneyCount.getTextDimensions().y);
}

void UserInterface::draw()
{
    drawHealthBar();
    m_healthPotion.draw();
    m_potionCount.draw();

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 200);

    SDL_Rect boomerangCoolDownCover{ g_screenWidth - 78, g_screenHeight - 78, 56, static_cast<int>(m_player->getRangedAttackCooldownFraction() * 56.0) };
    m_rangedCooldown.draw();
    SDL_RenderFillRect(g_renderer, &boomerangCoolDownCover);

    SDL_Rect dodgeCoolDownCover{ g_screenWidth - 158, g_screenHeight - 78, 56, static_cast<int>(m_player->getDodgeCooldownFraction() * 56.0) };
    m_dodgeCooldown.draw();
    SDL_RenderFillRect(g_renderer, &dodgeCoolDownCover);

    m_moneyIcon.draw();
    m_moneyCount.draw();

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_NONE);
}
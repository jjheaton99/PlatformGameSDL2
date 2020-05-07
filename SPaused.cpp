#include "SPaused.h"

SPaused::SPaused()
{
    m_pausedTexture.load("Assets/MiscTextures/paused.png");
}

SPaused::~SPaused()
{
    m_pausedTexture.free();
}

GameState::State SPaused::handleEvents()
{
    for (SDL_Event& element : m_events)
    {
        if (element.type == SDL_KEYDOWN)
        {
            switch (element.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                return PLAY_GAME;
            default:
                break;
            }
        }
    }

    return STATE_NULL;
}

GameState::State SPaused::update()
{
    m_pausedDstRect = { (g_screenWidth / 2) - 300, 150, 600, 120 };
    return STATE_NULL;
}

void SPaused::render()
{
    m_pausedTexture.draw(m_pausedSrcRect, m_pausedDstRect, 0, nullptr, SDL_FLIP_NONE);
}
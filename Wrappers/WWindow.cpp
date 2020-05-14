#include "WWindow.h"

WWindow::WWindow()
{}

WWindow::~WWindow()
{}

bool WWindow::init()
{
    m_window = SDL_CreateWindow("SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_screenWidth, g_screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowMinimumSize(m_window, 1600, 900);
    SDL_SetWindowMaximumSize(m_window, 1920, 1080);

    return m_window != nullptr;
}

SDL_Renderer* WWindow::createRenderer()
{
    return SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
}

void WWindow::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_WINDOWEVENT)
    {
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            g_screenWidth = event.window.data1;
            g_screenHeight = event.window.data2;
            SDL_RenderPresent(g_renderer);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(g_renderer);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            m_minimised = true;
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            m_minimised = false;
            break;
        case SDL_WINDOWEVENT_RESTORED:
            m_minimised = false;
            break;
        default:
            break;
        }
    }
}

void WWindow::fullscreenOnOff()
{
    switch (m_fullscreen)
    {
    case true:
        m_fullscreen = false;
        SDL_SetWindowFullscreen(m_window, 0);
        break;
    case false:
        m_fullscreen = true;
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        g_screenWidth = 1920;
        g_screenHeight = 1080;
        break;
    }
}

void WWindow::destroy()
{
    SDL_DestroyWindow(m_window);
    //doesnt like me deleting window pointer
}

void WWindow::setMouseCentre()
{
    SDL_WarpMouseInWindow(m_window, g_screenWidth / 2, g_screenHeight / 2);
}
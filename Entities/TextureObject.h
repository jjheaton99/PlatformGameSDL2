#pragma once

#include "SDL.h"
#include "Vector2D.h"
#include "WTexture.h"

//base class for all texture based objects
class TextureObject
{
protected:
    //double position vector used for precision over int
    Vector2D<double> m_position;
    double m_angle{ 0.0 };

    WTexture m_texture;
    SDL_Rect m_srcRect;
    SDL_Rect m_dstRect;

public:
    //fileName is the name of the texture png file to load
    TextureObject(std::string fileName, double xPos = 0, double yPos = 0);
    TextureObject(double xPos = 0, double yPos = 0);
    TextureObject(SDL_Color textColour, std::string text, double xPos = 0, double yPos = 0);
    virtual ~TextureObject();

    virtual void draw() const;
    //virtual void destroy();

    const Vector2D<double>& getPos() const { return m_position; }
    virtual void setPos(double xPos, double yPos) { m_position = Vector2D<double>{ xPos, yPos }; }
    void setSrcRect(int xPos, int yPos, int width, int height) { m_srcRect = { xPos, yPos, width, height }; }
    void setSrcRect(const SDL_Rect& rect) { m_srcRect = rect; }
    void setDstRect(int xPos, int yPos, int width, int height) { m_dstRect = { xPos, yPos, width, height }; }
    void setDstRect(const SDL_Rect& rect) { m_dstRect = rect; }
    const SDL_Rect& getDstRect() const { return m_dstRect; }

    void alphaBlendOn() { m_texture.setBlendingMode(SDL_BLENDMODE_BLEND); }
    void setAlpha(Uint8 alpha) { m_texture.setAlpha(alpha); }
    void setColour(Uint8 R, Uint8 G, Uint8 B) { m_texture.setColour(R, G, B); }

    void loadPng(std::string text) { m_texture.load(text); }
    void loadText(std::string text, SDL_Color colour);
    const SDL_Point& getTextDimensions() const { return m_texture.getTextDimensions(); }
};


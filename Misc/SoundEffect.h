#pragma once

#include "SDL_mixer.h"
#include "Constants.h"
#include "SSettings.h"
#include <string>
#include <iostream>

class SoundEffect
{
private:
    Mix_Chunk* m_soundEffect{ nullptr };
    int m_baseVolume;

public:
    SoundEffect(std::string soundFile, Uint8 volume = 128);
    SoundEffect();
    ~SoundEffect();

    void load(std::string soundFile, Uint8 volume = 128);
    void play();
    void setVolume(Uint8 volume);
};


#pragma once

#include "SDL_mixer.h"
#include <string>
#include <iostream>

class SoundEffect
{
private:
    Mix_Chunk* m_soundEffect;

public:
    SoundEffect(std::string soundFile);
    ~SoundEffect();

    void play() const;
    void setPercentVolume(double percent);
};


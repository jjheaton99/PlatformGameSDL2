#pragma once

#include "SDL_mixer.h"
#include <string>
#include <iostream>

class SoundEffect
{
private:
    Mix_Chunk* m_soundEffect{ nullptr };

public:
    SoundEffect(std::string soundFile);
    SoundEffect();
    ~SoundEffect();

    void load(std::string soundFile);
    void play() const;
    void setPercentVolume(double percent);
};


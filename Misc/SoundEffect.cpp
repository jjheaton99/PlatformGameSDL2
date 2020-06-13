#include "SoundEffect.h"

SoundEffect::SoundEffect(std::string soundFile)
{
    m_soundEffect = Mix_LoadWAV(soundFile.c_str());
    if (!m_soundEffect)
    {
        std::cout << "Unable to load sound! Mix_Error: " << Mix_GetError() << '\n';
    }
}

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(m_soundEffect);
}

void SoundEffect::play() const
{
    Mix_PlayChannel(-1, m_soundEffect, 0);
}

void SoundEffect::setPercentVolume(double percent)
{
    if (percent <= 0.0)
    {
        Mix_VolumeChunk(m_soundEffect, 0);
    }
    else if (percent >= 100.0)
    {
        Mix_VolumeChunk(m_soundEffect, MIX_MAX_VOLUME);
    }
    else
    {
        Mix_VolumeChunk(m_soundEffect, static_cast<int>((percent / 100.0) * static_cast<double>(MIX_MAX_VOLUME)));
    }
}
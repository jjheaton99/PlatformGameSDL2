#include "SoundEffect.h"

SoundEffect::SoundEffect(std::string soundFile, Uint8 volume)
    : m_baseVolume{volume}
{
    load(soundFile, volume);
}

SoundEffect::SoundEffect()
    : m_baseVolume{MIX_MAX_VOLUME}
{}

SoundEffect::~SoundEffect()
{
    Mix_FreeChunk(m_soundEffect);
}

void SoundEffect::load(std::string soundFile, Uint8 volume)
{
    m_baseVolume = volume;

    if (m_soundEffect)
    {
        Mix_FreeChunk(m_soundEffect);
    }

    m_soundEffect = Mix_LoadWAV(soundFile.c_str());
    if (!m_soundEffect)
    {
        std::cout << "Unable to load sound! Mix_Error: " << Mix_GetError() << '\n';
    }
    else
    {
        setVolume(volume);
    }
}

void SoundEffect::play()
{
    Mix_VolumeChunk(m_soundEffect, static_cast<Uint8>((Settings::sfxVol / 100.0) * (Settings::masterVol / 100.0) * m_baseVolume));
    Mix_PlayChannel(-1, m_soundEffect, 0);
}

void SoundEffect::setVolume(Uint8 volume)
{
    Mix_VolumeChunk(m_soundEffect, volume);
}
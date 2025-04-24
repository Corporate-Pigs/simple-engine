#include "SE_sdlSound.h"

#include <stdexcept>

void SimpleEngine::Sound::Start()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::string error = Mix_GetError();
        throw std::runtime_error("[SDLSound] Error open audio: " + error);
    }
}

void SimpleEngine::Sound::Cleanup()
{
    for (auto const &[key, sound] : m_soundEffectCache)
    {
        Mix_FreeChunk(sound);
    }

    for (auto const &[key, music] : m_musicCache)
    {
        Mix_FreeMusic(music);
    }

    Mix_CloseAudio();
}

void SimpleEngine::Sound::PlaySoundEffect(const SoundEffect &p_soundEffect)
{
    Mix_Chunk *soundEffectPtr = GetSDLSoundEffect(p_soundEffect.c_asset);
    Mix_PlayChannel(-1, soundEffectPtr, 0);
}

void SimpleEngine::Sound::PlayMusic(const Music &p_music)
{
    Mix_Music *musicPtr = GetSDLMusic(p_music.c_asset);
    Mix_PlayMusic(musicPtr, -1);
}

Mix_Chunk *SimpleEngine::Sound::CreateSDLSoundEffect(const std::string &p_assetPath)
{
    Mix_Chunk *soundEffectPtr = Mix_LoadWAV(p_assetPath.c_str());
    if (!soundEffectPtr)
    {
        std::string error = Mix_GetError();
        throw std::runtime_error("[SDLSound] Error creating sound effect from asset: " + p_assetPath + ". " + error);
    }
    m_soundEffectCache[p_assetPath] = soundEffectPtr;
    return soundEffectPtr;
}

Mix_Music *SimpleEngine::Sound::CreateSDLMusic(const std::string &p_assetPath)
{
    Mix_Music *musicPtr = Mix_LoadMUS(p_assetPath.c_str());
    if (!musicPtr)
    {
        std::string error = Mix_GetError();
        throw std::runtime_error("[SDLSound] Error creating music from asset: " + p_assetPath + ". " + error);
    }
    m_musicCache[p_assetPath] = musicPtr;
    return musicPtr;
}

Mix_Chunk *SimpleEngine::Sound::GetSDLSoundEffect(const std::string &p_assetPath)
{
    auto it = m_soundEffectCache.find(p_assetPath);
    if (it != m_soundEffectCache.end())
    {
        return it->second;
    }
    return CreateSDLSoundEffect(p_assetPath);
}

Mix_Music *SimpleEngine::Sound::GetSDLMusic(const std::string &p_assetPath)
{
    auto it = m_musicCache.find(p_assetPath);
    if (it != m_musicCache.end())
    {
        return it->second;
    }
    return CreateSDLMusic(p_assetPath);
}

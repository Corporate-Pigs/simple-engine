#pragma once

#include <SDL_mixer.h>

#include <unordered_map>

#include "SE_music.h"
#include "SE_soundEffect.h"

namespace SimpleEngine
{

struct Sound
{
    friend class Game;
    friend class Backend;

   public:
    void PlaySoundEffect(const SoundEffect &p_soundEffect);
    void PlayMusic(const Music &p_music);

   private:
    std::unordered_map<std::string, Mix_Chunk *> m_soundEffectCache;
    std::unordered_map<std::string, Mix_Music *> m_musicCache;

    void Start();
    void Cleanup();

    // Helper functions
    Mix_Chunk *CreateSDLSoundEffect(const std::string &p_assetPath);
    Mix_Music *CreateSDLMusic(const std::string &p_assetPath);
    Mix_Chunk *GetSDLSoundEffect(const std::string &p_assetPath);
    Mix_Music *GetSDLMusic(const std::string &p_assetPath);
};

}  // namespace SimpleEngine

#pragma once

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
    void Start();
    void Cleanup();
};

}  // namespace SimpleEngine

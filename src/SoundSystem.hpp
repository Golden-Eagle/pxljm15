#ifndef PXLJM_SOUND_HEADER
#define PXLJM_SOUND_HEADER

#include "ComponentSystem.hpp"

#include "fmod_studio.hpp"

namespace pxljm {
  class SoundSystem : public ComponentSystem {
    FMOD::Studio::System* m_system    = NULL;
    FMOD::Studio::Bank* m_masterBank  = NULL;
    FMOD::Studio::Bank* m_stringsBank = NULL;

    FMOD::Studio::EventDescription* m_testEvent = NULL;

    bool triggeredSound = false;
  public:
    SoundSystem();
    virtual ~SoundSystem();

    void update();
  };
}

#endif
#include "SoundSystem.hpp"

#include "fmod_studio.hpp"
#include "fmod_errors.h"

pxljm::SoundSystem::SoundSystem() {
  FMOD_RESULT result;

  result = FMOD::Studio::System::create(&m_system);
  if(result != FMOD_OK) {
    std::cout << "create() error: " << FMOD_ErrorString(result) << std::endl;
  }

  result = m_system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
  if(result != FMOD_OK) {
    std::cout << "initialize() error: " << FMOD_ErrorString(result) << std::endl;
  }

  result = m_system->loadBankFile("./res/sound/pxljm15/Build/Desktop/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &m_masterBank);
  if(result != FMOD_OK) {
    std::cout << "loadBankFile() error: " << FMOD_ErrorString(result) << std::endl;
  }
  result = m_system->loadBankFile("./res/sound/pxljm15/Build/Desktop/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &m_stringsBank);
  if(result != FMOD_OK) {
    std::cout << "loadBankFile() error: " << FMOD_ErrorString(result) << std::endl;
  }

  result = m_system->getEvent("event:/Test", &m_testEvent);
  if(result != FMOD_OK) {
    std::cout << "getEvent() error: " << FMOD_ErrorString(result) << std::endl;
  }
}

pxljm::SoundSystem::~SoundSystem() {
  m_system->release();
}

void pxljm::SoundSystem::update() {
  if(!triggeredSound) {
    triggeredSound = true;
    FMOD::Studio::EventInstance* eventInstance = NULL;
    FMOD_RESULT result;
    result = m_testEvent->createInstance(&eventInstance);
    if(result != FMOD_OK) {
      std::cout << "createInstance() error: " << FMOD_ErrorString(result) << std::endl;
    }
    result = eventInstance->start();
    if(result != FMOD_OK) {
      std::cout << "start() error: " << FMOD_ErrorString(result) << std::endl;
    }
  }

  m_system->update();
}
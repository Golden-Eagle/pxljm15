
#include "UpdateSystem.hpp"
#include "Scene.hpp"

using namespace std;
using namespace gecom;
using namespace i3d;
using namespace std::chrono_literals;


//
// Update component
//
void UpdateComponent::registerWith(Scene &s) { s.updateSystem().registerUpdateComponent(this); }


void UpdateComponent::deregisterWith(Scene &s) { s.updateSystem().deregisterUpdateComponent(this); }


chrono::duration<double> UpdateComponent::updateInterval() { return 0s; }



//
// Input Update component
//
void InputUpdateComponent::registerWith(Scene &s) { s.updateSystem().registerInputUpdateComponent(this); }


void InputUpdateComponent::deregisterWith(Scene &s) { s.updateSystem().deregisterInputUpdateComponent(this); }



// 
// UpdateComponent System
// 
UpdateSystem::UpdateSystem() { }


void UpdateSystem::registerUpdateComponent(UpdateComponent *c) {
	m_updatables.insert(c);
}


void UpdateSystem::deregisterUpdateComponent(UpdateComponent *c) {
	m_updatables.erase(c);
}


void UpdateSystem::update() { //TODO change this to update queue stuff
	for (UpdateComponent *c : m_updatables)
		c->update();
}


void UpdateSystem::registerInputUpdateComponent(InputUpdateComponent *c) {
	m_inputUpdatables.insert(c);
}


void UpdateSystem::deregisterInputUpdateComponent(InputUpdateComponent *c) {
	m_inputUpdatables.erase(c);
}


void UpdateSystem::inputUpdate() {
	for (InputUpdateComponent *c : m_inputUpdatables)
		c->inputUpdate();
}
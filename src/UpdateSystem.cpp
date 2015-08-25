
#include "UpdateSystem.hpp"
#include "Scene.hpp"

using namespace std;
using namespace pxljm;
using namespace i3d;
using namespace std::chrono_literals;


//
// Update component
//
void Updatable::registerWith(Scene &s) { s.updateSystem().registerUpdatable(this); }


void Updatable::deregisterWith(Scene &s) { s.updateSystem().deregisterUpdatable(this); }


chrono::duration<double> Updatable::updateInterval() { return 0s; }



//
// Input Update component
//
void InputUpdatable::registerWith(Scene &s) { s.updateSystem().registerInputUpdatable(this); }


void InputUpdatable::deregisterWith(Scene &s) { s.updateSystem().deregisterInputUpdatable(this); }



// 
// UpdateComponent System
// 
UpdateSystem::UpdateSystem() { }


void UpdateSystem::registerUpdatable(Updatable *c) {
	m_updatables.insert(c);
}


void UpdateSystem::deregisterUpdatable(Updatable *c) {
	m_updatables.erase(c);
}


void UpdateSystem::update() { //TODO change this to update queue stuff
	for (Updatable *c : m_updatables)
		c->update();
}


void UpdateSystem::registerInputUpdatable(InputUpdatable *c) {
	m_inputUpdatables.insert(c);
}


void UpdateSystem::deregisterInputUpdatable(InputUpdatable *c) {
	m_inputUpdatables.erase(c);
}


void UpdateSystem::inputUpdate() {
	for (InputUpdatable *c : m_inputUpdatables)
		c->inputUpdate();
}
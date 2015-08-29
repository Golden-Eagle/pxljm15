
#include "Scene.hpp"

using namespace std;
using namespace pxljm;
using namespace gecom;
using namespace i3d;

Scene::Scene(Window *win) : m_window(win) { }


Scene::~Scene() { }


void Scene::update() {

	// Input Update
	//
	m_updateSystem.inputUpdate();

	// Physics
	//
	m_physicsSystem.tick();

	// Sound
	//
	m_soundSystem.update();

	// Update
	//
	m_updateSystem.update();

	// Animation
	// Later Josh... later...
}



void Scene::add( entity_ptr e){
	e->registerWith(*this);
	m_entities.push_back(e);
}


CameraSystem & Scene::cameraSystem() { return m_cameraSystem; }


DrawableSystem & Scene::drawableSystem() { return m_drawableSystem; }


PhysicsSystem & Scene::physicsSystem() { return m_physicsSystem; }


UpdateSystem & Scene::updateSystem() { return m_updateSystem; }


SoundSystem & Scene::soundSystem() { return m_soundSystem; }


LightSystem & Scene::lightSystem() { return m_lightSystem; }



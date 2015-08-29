
#include "Scene.hpp"

using namespace std;
using namespace pxljm;
using namespace gecom;
using namespace i3d;

Scene::Scene(Window *win) : m_renderer(win), m_window(win) { }


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
	m_camera.update();
	m_updateSystem.update();

	// Animation
	// Later Josh... later...
}

void Scene::render() {
	// Render
	//
	double zfar = 200.0;
	auto size = m_window->size();
	int w = size.w;
	int h = size.h;

	if (w != 0 && h != 0) {
		m_renderer.renderScene(*this);
		m_physicsSystem.debugDraw(*this);
	}
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



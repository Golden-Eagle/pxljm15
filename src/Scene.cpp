
#include "Scene.hpp"

using namespace std;
using namespace gecom;
using namespace i3d;

Scene::Scene(Window *win) : m_camera(win, i3d::vec3d(0, 2, 3)), m_renderer(win), m_window(win) { }


Scene::~Scene() { }


void Scene::tick() {

	// Input Update
	// 
	m_updateSystem.inputUpdate();

	// Physics
	// 
	m_physicsSystem.tick();

	// Update
	// 
	m_camera.update();
	m_updateSystem.update();

	// Animation
	// Later Josh... later...

	// Render
	// 
	double zfar = 200.0;
	auto size = m_window->size();
	int w = size.w;
	int h = size.h;

	if (w != 0 && h != 0) {

		m_projection.setPerspectiveProjection(i3d::math::pi() / 3, double(w) / h, 0.1, zfar);
		i3d::mat4d proj_matrix = m_projection.getProjectionTransform();
		i3d::mat4d view_matrix = m_camera.getViewTransform();

		std::priority_queue<drawcall *> drawQueue = m_drawableSystem.getDrawQueue(view_matrix);
		m_renderer.renderScene(proj_matrix, drawQueue);
	}
}


void Scene::add( entity_ptr e){
	e->registerWith(*this);
	m_entities.push_back(e);
}


DrawableSystem & Scene::drawableSystem() { return m_drawableSystem; }


PhysicsSystem & Scene::physicsSystem() { return m_physicsSystem; }


UpdateSystem & Scene::updateSystem() { return m_updateSystem; }


LightSystem & Scene::lightSystem() { return m_lightSystem; }

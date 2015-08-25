
#include "DrawSystem.hpp"
#include "Scene.hpp"


using namespace std;
using namespace pxljm;
using namespace i3d;



drawcall::~drawcall() { }


material_ptr drawcall::material() { return m_mat; }


bool drawcall::operator< (const drawcall& rhs) const {
	return m_mat->shader < rhs.m_mat->shader || m_mat < rhs.m_mat;
}


void DrawableComponent::registerWith(Scene &s) { s.drawableSystem().registerDrawableComponent(this); }


void DrawableComponent::deregisterWith(Scene &s) { s.drawableSystem().deregisterDrawableComponent(this); }



DrawableSystem::DrawableSystem() { }


void DrawableSystem::registerDrawableComponent(DrawableComponent *c) {
	m_drawables.insert(c);
}


void DrawableSystem::deregisterDrawableComponent(DrawableComponent *c) {
	m_drawables.erase(c);
}


priority_queue<drawcall *> DrawableSystem::getDrawQueue(mat4d viewMatrix) {
	priority_queue<drawcall *> drawQueue;
	for (DrawableComponent *drawable : m_drawables) {
		vector<drawcall *> drawcalls = drawable->getDrawCalls(viewMatrix);

		for (drawcall * draw : drawcalls)
			drawQueue.push(draw);
	}
	return drawQueue;
}

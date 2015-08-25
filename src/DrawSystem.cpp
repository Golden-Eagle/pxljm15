
#include "DrawSystem.hpp"
#include "Scene.hpp"


using namespace std;
using namespace pxljm;
using namespace i3d;



DrawCall::~DrawCall() { }


material_ptr DrawCall::material() { return m_mat; }


bool DrawCall::operator< (const DrawCall& rhs) const {
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


priority_queue<DrawCall *> DrawableSystem::getDrawQueue(mat4d viewMatrix) {
	priority_queue<DrawCall *> drawQueue;
	for (DrawableComponent *drawable : m_drawables) {
		vector<DrawCall *> DrawCalls = drawable->getDrawCalls(viewMatrix);

		for (DrawCall * draw : DrawCalls)
			drawQueue.push(draw);
	}
	return drawQueue;
}

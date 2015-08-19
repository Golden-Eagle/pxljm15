#include "Entity.hpp"
#include "Scene.hpp"

using namespace std;
using namespace gecom;
using namespace i3d;



//
// Entity
//
Entity::Entity(vec3d pos, quatd rot) {
	m_root = EntityTransform(pos, rot);
	m_components.push_back(&m_root);
}


Entity::Entity(quatd rot) {
	m_root = EntityTransform(rot);
	m_components.push_back(&m_root);
}


Entity::~Entity() {
	// Remove all compoenents from scene
	// if (m_scene)
	// 	for (EntityComponent *ec : m_components)

	// 		m_scene.deregisterComponent(ec);
}


void Entity::registerWith(Scene &s) {
	m_scene = &s;
}


void Entity::removeComponent(EntityComponent *) {
	// auto it = find(m_components.begin(), m_components.end(), ecp);
	// if (it == m_components.end()) {
	// 	if (m_scene)
	// 		(*it).registerWith(m_scene);
	// 	m_components.erase();
	// }
}


EntityTransform * Entity::root() {
	return &m_root;
}

const std::vector<EntityComponent *> & Entity::getAllComponents() const {
	return m_components;
}



//
// Entity component
//
EntityComponent::~EntityComponent() { }


bool EntityComponent::hasParent() { return bool(m_parent.lock()); }


entity_ptr EntityComponent::getParent() const { return m_parent.lock(); }


void EntityComponent::registerWith(Scene &) { }



//
// Update component
//
void UpdateComponent::registerWith(Scene &s) {
	s.registerUpdateComponent(this);
}



//
// Transform component
//
void TransformComponent::registerWith(Scene &) { }



//
// Entity Transform component
//
EntityTransform::EntityTransform(vec3d pos, quatd rot) : position(pos), rotation(rot) { }


EntityTransform::EntityTransform(quatd rot) : position(vec3d()), rotation(rot) { }


mat4d EntityTransform::matrix() {
	return mat4d::translate(position) * mat4d::rotate(rotation);
}


mat4d EntityTransform::localMatrix() {
	if (auto p = getParent())
		return p->root()->matrix() * matrix();
	return matrix();
}



//
// Drawable component
//
drawcall::~drawcall() { }


material_ptr drawcall::material() { return m_mat; }


bool drawcall::operator< (const drawcall& rhs) const {
	return m_mat->shader < rhs.m_mat->shader || m_mat < rhs.m_mat;
}


void DrawableComponent::registerWith(Scene &s) {
	s.registerDrawableComponent(this);
}



// Mesh Drawable
//
mesh_drawcall::mesh_drawcall(mat4d mv, material_ptr mat, mesh_ptr mesh)
	: m_mv(mv), m_mesh(mesh) { m_mat = mat; }


void mesh_drawcall::draw() {
	m_mesh->bind();
	glUniformMatrix4fv(m_mat->shader->uniformLocation("uModelViewMatrix"), 1, true, m_mv);
	m_mesh->draw();
}


MeshDrawable::MeshDrawable(mesh_ptr m, material_ptr mat) :
	mesh(m), material(mat), m_cachedDrawcall(mat4d(), mat, m) {  }


MeshDrawable::~MeshDrawable() { }


vector<drawcall *> MeshDrawable::getDrawCalls(mat4d view) {
	vector<drawcall *> drawcallList;
	m_cachedDrawcall = mesh_drawcall(view * getParent()->root()->matrix(), material, mesh);
	drawcallList.push_back(&m_cachedDrawcall);
	return drawcallList;
}




//
// Light component
//
LightComponent::~LightComponent() { }



void LightComponent::registerWith(Scene &s) {
	s.registerLightComponent(this);
}



// Directional Light
//
DirectionalLight::DirectionalLight() { }



// Point Light
//
PointLight::PointLight() { }



// Spot Light
//
SpotLight::SpotLight() { }



// 
// Component System
// 
ComponentSystem::~ComponentSystem() { }



// 
// Drawable System
// 
DrawableSystem::DrawableSystem() { }


DrawableSystem::~DrawableSystem() { }


void DrawableSystem::addDrawable(DrawableComponent * d) {
	m_drawables.push_back(d);
}

//TODO
void DrawableSystem::removeDrawable(DrawableComponent *) { }


priority_queue<drawcall *> DrawableSystem::getDrawQueue(mat4d viewMatrix) {
	priority_queue<drawcall *> drawQueue;
	for (DrawableComponent *drawable : m_drawables) {
		vector<drawcall *> drawcalls = drawable->getDrawCalls(viewMatrix);

		for (drawcall * draw : drawcalls)
			drawQueue.push(draw);
	}
	return drawQueue;
}



// 
// Lighting System
// 
LightSystem::LightSystem() { }


LightSystem::~LightSystem() { }


void LightSystem::addLight(LightComponent *light) {
	m_lights.push_back(light);
}

//TODO
void LightSystem::removeLight(LightComponent *) { }


vector<LightComponent *> LightSystem::getLights() {
	vector<LightComponent *> lightList;
	for (LightComponent *light : m_lights)
		lightList.push_back(light);
	return lightList;
}
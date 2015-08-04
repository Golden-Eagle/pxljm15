#include "Entity.hpp"

using namespace std;
using namespace gecom;

//
// Entity
//
Entity::Entity() { m_components.push_back(m_root); }


Entity::~Entity() {}


void Entity::addComponent( entity_comp_ptr ec ) {
	// when optimized scene
	// check to see if in current scene
	// if so, register component with current scene

	// add to entity
	auto it = find(m_components.begin(), m_components.end(), ec);
	if (it == m_components.end()) 
		m_components.push_back(ec);

	ec->m_parent = shared_from_this();
}


entity_tran_ptr Entity::root() const {
	return m_root;
}


const vector<entity_comp_ptr> & Entity::getComponents() const {
	return m_components;
}


template<typename T>
vector<shared_ptr<T>> Entity::getComponent() const {
	vector<T> cl;
	for (entity_comp_ptr c : m_components)
		if (auto i = dynamic_pointer_cast<T>(c))
			cl.push_back(i);
	return cl;
}



//
// Entity component
//
EntityComponent::EntityComponent() { }


EntityComponent::~EntityComponent() { }


void EntityComponent::registerTo(ComponentSystem *cs) { }


void EntityComponent::registerTo(DrawableSystem *ds) { }


void EntityComponent::update(Scene &s) { }


bool EntityComponent::hasParent() { return bool(m_parent.lock()); }


entity_ptr EntityComponent::getParent() const { return m_parent.lock(); }



//
// Tranform component
//
Transform::Transform() : m_transform(i3d::mat4d()) { }


i3d::mat4d Transform::matrix() {
	return m_transform;
}



//
// Drawable component
//
Drawable::Drawable() { }


void Drawable::registerTo(DrawableSystem *ds) {
	ds->registerDrawable(static_pointer_cast<Drawable>(shared_from_this()));
}



// Mesh Drawable
//
MeshDrawable::MeshDrawable() { }


mesh_ptr MeshDrawable::getMaterial() { return material; }


void MeshDrawable::draw(i3d::mat4f worldViewMat, i3d::mat4f projMat) {
	glUniformMatrix4fv(glGetUniformLocation(prog, "projectionMatrix"), 1, true, i3d::mat4f(projMat));
	glUniformMatrix4fv(glGetUniformLocation(prog, "modelViewMatrix"), 1, true, worldViewMat * getParent()->root()->matrix());
	mesh.bind();
	mesh.draw();
}



// 
// ComponentSystem
// 
ComponentSystem::~ComponentSystem() { }



// 
// DrawableSystem
// 
DrawableSystem::DrawableSystem() { }


DrawableSystem::~DrawableSystem() { }


void DrawableSystem::addComponent(entity_comp_ptr c){
	c->registerTo(static_cast<DrawableSystem *>(this));
}


void DrawableSystem::registerDrawable(entity_draw_ptr d) {
	m_drawables.push_back(d);
	cout << "registered!" << endl;
}


vector< entity_draw_ptr > DrawableSystem::getDrawList() {
	vector< entity_draw_ptr > drawList;
	for (auto it = m_drawables.begin() ; it != m_drawables.end(); ) {
		if (auto drawable = (*it).lock()) {
			drawList.push_back(drawable);
			++it;
		} else {
			it = m_drawables.erase(it);
		}
	}
	return drawList;
}
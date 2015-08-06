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
void Drawable::registerTo(DrawableSystem *ds) {
	ds->registerDrawable(static_pointer_cast<Drawable>(shared_from_this()));
}


material_ptr Drawable::drawcall::material() { return m_mat; }


bool Drawable::drawcall::operator< (const drawcall& rhs) const {
	return (m_mat->shader < rhs.m_mat->shader) ? true : (m_mat < rhs.m_mat);
}



// Mesh Drawable
//
MeshDrawable::MeshDrawable() { }


vector<Drawable::drawcall *> MeshDrawable::getDrawCalls(i3d::mat4d view) {
	vector<Drawable::drawcall *> dc;
	Drawable::drawcall * mdc = new MeshDrawable::mesh_drawcall(view * getParent()->root()->matrix(), material, mesh);
	dc.push_back(mdc);
	return dc;
}


MeshDrawable::mesh_drawcall::mesh_drawcall(i3d::mat4d mv, material_ptr mat, mesh_ptr mesh)
	: m_mv(mv), m_mesh(mesh) { m_mat = mat; }


void MeshDrawable::mesh_drawcall::draw() {
	m_mesh->bind();
	glUniformMatrix4fv(glGetUniformLocation(m_mat->shader->prog, "modelViewMatrix"), 1, true, m_mv);
	m_mesh->draw();
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


vector<Drawable::drawcall *> DrawableSystem::getDrawList(i3d::mat4d viewMatrix) {
	vector<Drawable::drawcall *> drawList;
	for (auto it = m_drawables.begin() ; it != m_drawables.end(); ) {
		if (auto drawable = (*it).lock()) {
			auto drawcalls = drawable->getDrawCalls(viewMatrix);
			drawList.insert(drawList.end(), drawcalls.begin(), drawcalls.end());
			++it;
		} else {
			it = m_drawables.erase(it);
		}
	}
	return drawList;
}
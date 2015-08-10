#include "Entity.hpp"

using namespace std;
using namespace gecom;
using namespace i3d;



//
// Entity
//
Entity::Entity(vec3d pos, quatd rot) {
	m_root = std::make_shared<EntityTransform>(pos, rot);
	m_components.push_back(m_root);
}


Entity::Entity(quatd rot) {
	m_root = std::make_shared<EntityTransform>(vec3d(), rot);
	m_components.push_back(m_root);
}


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


entity_transform_ptr Entity::root() const {
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


void EntityComponent::registerTo(ComponentSystem *) { }


void EntityComponent::registerTo(DrawableSystem *) { }


void EntityComponent::update(Scene &) { }


bool EntityComponent::hasParent() { return bool(m_parent.lock()); }


entity_ptr EntityComponent::getParent() const { return m_parent.lock(); }



//
// Tranform component
//



// Nothing here



//
// Entity Tranform component
//
EntityTransform::EntityTransform() { }


EntityTransform::EntityTransform(vec3d pos, quatd rot) : position(pos), rotation(rot) { }


EntityTransform::EntityTransform(quatd rot) : position(vec3d()), rotation(rot) { }


EntityTransform::~EntityTransform() { }


mat4d EntityTransform::matrix() {
	return mat4d::translate(position) * mat4d::rotate(rotation);
}


mat4d EntityTransform::localMatrix() {
	//TODO 
	// entity_ptr p = getParent();
	// if (p){
	// 	return 
	// }
	return mat4d::translate(position) * mat4d::rotate(rotation);
}



//
// Drawable component
//
void Drawable::registerTo(DrawableSystem *ds) {
	ds->registerDrawable(static_pointer_cast<Drawable>(shared_from_this()));
}


Drawable::drawcall::~drawcall() { }


material_ptr Drawable::drawcall::material() { return m_mat; }


bool Drawable::drawcall::operator< (const drawcall& rhs) const {
	return m_mat->shader < rhs.m_mat->shader || m_mat < rhs.m_mat;
}



// Mesh Drawable
//
MeshDrawable::MeshDrawable(mesh_ptr m, material_ptr mat) : mesh(m), material(mat) { }


vector<Drawable::drawcall *> MeshDrawable::getDrawCalls(mat4d view) {
	vector<Drawable::drawcall *> dc;
	Drawable::drawcall * mdc = new MeshDrawable::mesh_drawcall(view * getParent()->root()->matrix(), material, mesh);
	dc.push_back(mdc);
	return dc;
}


MeshDrawable::mesh_drawcall::mesh_drawcall(mat4d mv, material_ptr mat, mesh_ptr mesh)
	: m_mv(mv), m_mesh(mesh) { m_mat = mat; }


void MeshDrawable::mesh_drawcall::draw() {
	m_mesh->bind();
	glUniformMatrix4fv(m_mat->shader->uniformLocation("uModelViewMatrix"), 1, true, m_mv);
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


vector<Drawable::drawcall *> DrawableSystem::getDrawList(mat4d viewMatrix) {
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
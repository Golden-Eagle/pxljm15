#include "Entity.hpp"
#include "Scene.hpp"

using namespace std;
using namespace pxljm;
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
	// 	for (EntityComponent *c : m_components)
	// 		c->deregisterWith(*m_scene);
}


void Entity::registerWith(Scene &s) {
	m_scene = &s;

	for (EntityComponent *c : m_components)
		c->registerWith(s);
}

void Entity::deregister() {
	for (EntityComponent *c : m_components)
		c->deregisterWith(*m_scene);
	m_scene = nullptr;
}


void Entity::addComponent(std::unique_ptr<EntityComponent> ec) {
	EntityComponent *ecp = ec.get();

	m_dynamicComponents.push_back(std::move(ec));
	m_components.push_back(ecp);

	ecp->m_entity = shared_from_this();

	ecp->start();

	if (m_scene)
		ecp->registerWith(*m_scene);
}


void Entity::removeComponent(EntityComponent *c) {
	auto it = find(m_components.begin(), m_components.end(), c);
	if (it == m_components.end()) {
		if (m_scene)
			(*it)->deregisterWith(*m_scene);
		m_components.erase(it);
	}
}


EntityTransform * Entity::root() {
	return &m_root;
}

const std::vector<EntityComponent *> & Entity::getAllComponents() const {
	return m_components;
}



//
// Transform component
//
void TransformComponent::registerWith(Scene &) { }


void TransformComponent::deregisterWith(Scene &) { }



//
// Entity Transform component
//
EntityTransform::EntityTransform(vec3d pos, quatd rot) : position(pos), rotation(rot) { }


EntityTransform::EntityTransform(quatd rot) : position(vec3d()), rotation(rot) { }


mat4d EntityTransform::matrix() {
	if (m_parent) {
		return m_parent->matrix() * mat4d::translate(position) * mat4d::rotate(rotation);
	}
	return mat4d::translate(position) * mat4d::rotate(rotation);
}


mat4d EntityTransform::localMatrix() {
	return mat4d::translate(position) * mat4d::rotate(rotation);
}


vec3d EntityTransform::getPosition() const {
	if (m_parent) {
		return vec3d(m_parent->matrix() * vec4d(position));
	}
	return position;
}


quatd EntityTransform::getRotation() const {
	if (m_parent) {
		return m_parent->getRotation() * rotation;
	}
	return rotation;
}


vec3d EntityTransform::getLocalPosition() const {
	return position;
}


quatd EntityTransform::getLocalRotation() const {
	return rotation;
}


void EntityTransform::setPosition(vec3d pos) {
	if (m_parent) {
		position = !m_parent->matrix() * pos;
	} else {
		position = pos;
	}
}


void EntityTransform::setRotation(quatd rot) {
	if (m_parent) {
		rotation = !m_parent->getRotation() * rot;
	}
	else {
		rotation = rot;
	}
}

void EntityTransform::setLocalPosition(vec3d pos) {
	position = pos;
}

void EntityTransform::setLocalRotation(quatd rot) {
	rotation = rot;
}


void EntityTransform::addChild(EntityTransform *c) {
	m_children.insert(c);
	if (c->m_parent) {
		c->m_parent->removeChild(c);
	}
	c->m_parent = this;
}


void EntityTransform::removeChild(EntityTransform *c) {
	m_children.erase(c);
	c->m_parent = nullptr;
}


bool EntityTransform::hasParent() {
	return m_parent;
}

EntityTransform * EntityTransform::getParent() {
	return m_parent;
}


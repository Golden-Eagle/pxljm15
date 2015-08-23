#include "Entity.hpp"
#include "Scene.hpp"

using namespace std;
using namespace gecom;
using namespace i3d;
using namespace std::chrono_literals;


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
	if (m_scene)
		for (EntityComponent *c : m_components)
			c->deregisterWith(*m_scene);
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
// Entity component
//
EntityComponent::~EntityComponent() { }


bool EntityComponent::hasEntity() { return bool(m_entity.lock()); }


entity_ptr EntityComponent::entity() const { return m_entity.lock(); }


void EntityComponent::start() { }


void EntityComponent::registerWith(Scene &) { }


void EntityComponent::deregisterWith(Scene &) { }



//
// Update component
//
void UpdateComponent::registerWith(Scene &s) { s.updateSystem().registerUpdateComponent(this); }


void UpdateComponent::deregisterWith(Scene &s) { s.updateSystem().deregisterUpdateComponent(this); }


chrono::duration<double> UpdateComponent::updateInterval() { return 0s; }



//
// Input Update component
//
void InputUpdateComponent::registerWith(Scene &s) { s.updateSystem().registerInputUpdateComponent(this); }


void InputUpdateComponent::deregisterWith(Scene &s) { s.updateSystem().deregisterInputUpdateComponent(this); }



//
// Physics Update component
//
void PhysicsUpdateComponent::registerWith(Scene &s) { s.physicsSystem().registerPhysicsUpdateComponent(this); }

void PhysicsUpdateComponent::deregisterWith(Scene &s) { s.physicsSystem().deregisterPhysicsUpdateComponent(this); }



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
	return mat4d::translate(position) * mat4d::rotate(rotation);
}


vec3d EntityTransform::getPosition() const {
	return position;
}


quatd EntityTransform::getRotation() const {
	return rotation;
}


void EntityTransform::setPosition(vec3d pos) {
	position = pos;
}


void EntityTransform::setRotation(quatd rot) {
	rotation = rot;
}


// mat4d EntityTransform::localMatrix() {
// 	return matrix();
// }



//
// Drawable component
//
drawcall::~drawcall() { }


material_ptr drawcall::material() { return m_mat; }


bool drawcall::operator< (const drawcall& rhs) const {
	return m_mat->shader < rhs.m_mat->shader || m_mat < rhs.m_mat;
}


void DrawableComponent::registerWith(Scene &s) { s.drawableSystem().registerDrawableComponent(this); }


void DrawableComponent::deregisterWith(Scene &s) { s.drawableSystem().deregisterDrawableComponent(this); }



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
	m_cachedDrawcall = mesh_drawcall(view * entity()->root()->matrix(), material, mesh);
	drawcallList.push_back(&m_cachedDrawcall);
	return drawcallList;
}



//
// Physics component
//
PhysicsComponent::~PhysicsComponent() { }


void PhysicsComponent::registerWith(Scene &s) { s.physicsSystem().registerPhysicsComponent(this); }


void PhysicsComponent::deregisterWith(Scene &s) { s.physicsSystem().deregisterPhysicsComponent(this); }



// Rigid Body component
//
RigidBody::RigidBody() {
	abort(); // LOL wut u doin?!?!
}

RigidBody::RigidBody(collider_ptr c) {
	m_collider = c;
}

void RigidBody::start() {
	regenerateRigidBody();
}


void RigidBody::addToDynamicsWorld(btDynamicsWorld * world) {
	removeFromDynamicsWorld();
	m_world = world;
	m_world->addRigidBody(m_rigidBody.get());
}


void RigidBody::removeFromDynamicsWorld() {
	if (m_world) {
		m_world->removeRigidBody(m_rigidBody.get());
		m_world = nullptr;
	}
}


void RigidBody::setCollider(collider_ptr col) {
	if (m_world)
		m_world->removeRigidBody(m_rigidBody.get());

	m_collider = col;

	btCollisionShape *shape = m_collider->getCollisionShape();
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(m_mass, inertia);

	m_rigidBody->setMassProps(m_mass, inertia);
	m_rigidBody->updateInertiaTensor();

	if (m_world)
		m_world->addRigidBody(m_rigidBody.get());
}


collider_ptr RigidBody::getCollider() {
	return m_collider;
}


void RigidBody::getWorldTransform (btTransform &centerOfMassWorldTrans) const {
	vec3d pos = entity()->root()->getPosition();
	quatd rot = entity()->root()->getRotation();
	centerOfMassWorldTrans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));
	centerOfMassWorldTrans.setRotation(btQuaternion(rot.x(), rot.y(), rot.z(), rot.w()));
}


void RigidBody::setWorldTransform (const btTransform &centerOfMassWorldTrans) {
	btVector3 pos = centerOfMassWorldTrans.getOrigin();
	btQuaternion rot = centerOfMassWorldTrans.getRotation();
	entity()->root()->setPosition(vec3d(pos.getX(), pos.getY(), pos.getZ()));
	entity()->root()->setRotation(quatd(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));
}


void RigidBody::regenerateRigidBody() {
	btCollisionShape *shape = m_collider->getCollisionShape();
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(m_mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, this, shape, inertia);
	m_rigidBody = make_unique<btRigidBody>(rigidBodyCI);
}



//
// Light component
//
LightComponent::~LightComponent() { }


void LightComponent::registerWith(Scene &s) { s.lightSystem().registerLightComponent(this); }


void LightComponent::deregisterWith(Scene &s) { s.lightSystem().deregisterLightComponent(this); }



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
// UpdateComponent System
// 
UpdateSystem::UpdateSystem() { }


void UpdateSystem::registerUpdateComponent(UpdateComponent *c) {
	m_updatables.insert(c);
}


void UpdateSystem::deregisterUpdateComponent(UpdateComponent *c) {
	m_updatables.erase(c);
}


void UpdateSystem::update() { //TODO change this to update queue stuff
	for (UpdateComponent *c : m_updatables)
		c->update();
}


void UpdateSystem::registerInputUpdateComponent(InputUpdateComponent *c) {
	m_inputUpdatables.insert(c);
}


void UpdateSystem::deregisterInputUpdateComponent(InputUpdateComponent *c) {
	m_inputUpdatables.erase(c);
}


void UpdateSystem::inputUpdate() {
	for (InputUpdateComponent *c : m_inputUpdatables)
		c->inputUpdate();
}



// 
// Drawable System
// 
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



// 
// Physical System
//
static void physicsSystemTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    PhysicsSystem *w = static_cast<PhysicsSystem *>(world->getWorldUserInfo());
    w->processPhysicsCallback(timeStep);
}


PhysicsSystem::PhysicsSystem() {

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	// Callback
	dynamicsWorld->setInternalTickCallback(physicsSystemTickCallback, static_cast<void *>(this));

	// TODO make it realitive?
	// If not configurable
	// HACK
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));


	// SUPER HACK
	// Hard limit for the ground to be at y=0
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

}


PhysicsSystem::~PhysicsSystem() {
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}


void PhysicsSystem::registerPhysicsUpdateComponent(PhysicsUpdateComponent * c) {
	m_physicsUpdatables.insert(c);
}


void PhysicsSystem::deregisterPhysicsUpdateComponent(PhysicsUpdateComponent *c) {
	m_physicsUpdatables.erase(c);
}


void PhysicsSystem::registerPhysicsComponent(PhysicsComponent * c) {
	m_rigidbodies.insert(c);
	c->addToDynamicsWorld(dynamicsWorld);
}


void PhysicsSystem::deregisterPhysicsComponent(PhysicsComponent *c) {
	c->removeFromDynamicsWorld();
	m_rigidbodies.erase(c);
}


void PhysicsSystem::tick() {
	dynamicsWorld->stepSimulation(1 / 60.f, 10);
}


void PhysicsSystem::processPhysicsCallback(btScalar timeStep) {
	for (PhysicsUpdateComponent *pc : m_physicsUpdatables)
		pc->physicsUpdate();
}



// 
// Lighting System
// 
LightSystem::LightSystem() { }


void LightSystem::registerLightComponent(LightComponent *c) {
	m_lights.insert(c);
}


void LightSystem::deregisterLightComponent(LightComponent *c) {
	m_lights.erase(c);
}


const unordered_set<LightComponent *> & LightSystem::getLights() {
	return m_lights;
}
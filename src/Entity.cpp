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



// //
// // Physics Update component
// //
// void PhysicsUpdateComponent::registerWith(Scene &s) { s.physicsSystem().registerPhysicalComponent(this); }

// void PhysicsUpdateComponent::deregisterWith(Scene &s) { s.physicsSystem().deregisterPhysicalComponent(this); }



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


mat4d EntityTransform::localMatrix() {
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



// //
// // Physical component
// //
// PhysicalComponent::~PhysicalComponent() { }


// void PhysicalComponent::registerWith(Scene &s) {
// 	s.registerPhysicalComponent(this);
// }


// void PhysicalComponent::updateTransform() {
// 	btTransform trans;
// 	rigidBody->getMotionState()->getWorldTransform(trans);
// 	btVector3 pos = trans.getOrigin();
// 	entity()->root()->position = vec3d(pos.getX(), pos.getY(), pos.getZ());
// 	cout << "Position now at " << entity()->root()->position << endl;
// }


// // Rigid Body component
// //
// RigidBody::RigidBody() { }


// RigidBody::~RigidBody() { }

// void PhysicalComponent::registerWith(Scene &s) {
// 	vec3d pos = entity()->root()->position;
// 	quatd rot = entity()->root()->rotation;

// 	// Shape
// 	btCollisionShape *shape = m_collider.collisionShape();

// 	// Motion state
// 	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(
// 		btQuaternion(rot.x(), rot.y(), rot.z(), rot.w()), btVector3(pos.x(), pos.y(), pos.z())));

// 	// Specifics
// 	btScalar mass = 1;
// 	btVector3 inertia(0, 0, 0);
// 	shape->calculateLocalInertia(mass, inertia);
// 	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, shape, inertia);
// 	rigidBody = new btRigidBody(rigidBodyCI);

// 	s.registerPhysicalComponent(this);
// }


// void RigidBody::updateTransform() {

// }



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



// // 
// // Physical System
// // 
// PhysicalSystem::PhysicalSystem() {

// 	broadphase = new btDbvtBroadphase();
// 	collisionConfiguration = new btDefaultCollisionConfiguration();
// 	dispatcher = new btCollisionDispatcher(collisionConfiguration);
// 	solver = new btSequentialImpulseConstraintSolver();

// 	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

// 	// TODO make it realitive?
// 	// If not configurable
// 	// HACK
// 	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));


// 	// HACK
// 	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), -5);
// 	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
// 	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
// 	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
// 	dynamicsWorld->addRigidBody(groundRigidBody);

// }


// PhysicalSystem::~PhysicalSystem() {
// 	delete dynamicsWorld;

// 	delete solver;
// 	delete dispatcher;
// 	delete collisionConfiguration;
// 	delete broadphase;
// }


// void PhysicalSystem::addPhysics(PhysicalComponent * d) {
// 	m_rigidbodies.push_back(d);
// 	dynamicsWorld->addRigidBody(d->rigidBody);
// }

// //TODO
// void PhysicalSystem::removePhysics(PhysicalComponent *) { }


// void PhysicalSystem::tick() {
// 	dynamicsWorld->stepSimulation(1 / 60.f, 10);
// 	for (PhysicalComponent *pc : m_rigidbodies)
// 		pc->updateTransform();
// }



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
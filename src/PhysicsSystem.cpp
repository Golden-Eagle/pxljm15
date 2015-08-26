#include "PhysicsSystem.hpp"
#include "Scene.hpp"


using namespace std;
using namespace pxljm;
using namespace gecom;
using namespace i3d;


//
// Physics Update component
//
void PhysicsUpdatable::registerWith(Scene &s) { s.physicsSystem().registerPhysicsUpdatable(this); }

void PhysicsUpdatable::deregisterWith(Scene &s) { s.physicsSystem().deregisterPhysicsUpdatable(this); }






//
// Physics component
//
// Rigid Body component
//
void RigidBody::registerWith(Scene &s) { s.physicsSystem().registerRigidBody(this); }


void RigidBody::deregisterWith(Scene &s) { s.physicsSystem().deregisterRigidBody(this); }


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


btRigidBody * RigidBody::getRigidBody() {
	return m_rigidBody.get();
}


void RigidBody::getWorldTransform (btTransform &centerOfMassWorldTrans) const {
	vec3d pos = entity()->root()->getPosition();
	quatd rot = entity()->root()->getRotation();
	centerOfMassWorldTrans.setOrigin(i3d2bt(pos));
	centerOfMassWorldTrans.setRotation(i3d2bt(rot));
}


void RigidBody::setWorldTransform (const btTransform &centerOfMassWorldTrans) {
	btVector3 pos = centerOfMassWorldTrans.getOrigin();
	btQuaternion rot = centerOfMassWorldTrans.getRotation();
	entity()->root()->setPosition(bt2i3d(pos));
	entity()->root()->setRotation(bt2i3d(rot));
}


void RigidBody::regenerateRigidBody() {
	// Create rigid body
	btCollisionShape *shape = m_collider->getCollisionShape();
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(m_mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass, this, shape, inertia);
	m_rigidBody = make_unique<btRigidBody>(rigidBodyCI);

	// Set Attributes
	// static cast to physics component pointer be cause the type needs to be
	// exactly what we reinterpret_cast it as, in the collision handler
	m_rigidBody->setUserPointer(static_cast<Physical *>(this)); // ben figured this out.. ask him why
}




//
// Collision Callback component
//
void CollisionCallback::registerWith(Scene &s) { s.physicsSystem().registerCollisionCallback(this); }


void CollisionCallback::deregisterWith(Scene &s) { s.physicsSystem().deregisterCollisionCallback(this); }


void CollisionCallback::onCollisionEnter(Physical *) { }


void CollisionCallback::onCollision(Physical *) { }


void CollisionCallback::onCollisionExit(Physical *) { }





PhysicsDebugDrawer::PhysicsDebugDrawer() {
	// setup the buffer!

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_pos);
	glGenBuffers(1, &m_vbo_col);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_col);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	m_shader = assets::getShader("physics_debug");

}


void PhysicsDebugDrawer::draw(mat4d view, mat4d proj) {
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_position.size(), &m_position[0], GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_col);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_color.size(), &m_color[0], GL_STREAM_DRAW);

	m_shader->bind();
	glUniformMatrix4fv(m_shader->uniformLocation("uModelViewMatrix"), 1, true, i3d::mat4f(view));
	glUniformMatrix4fv(m_shader->uniformLocation("uProjectionMatrix"), 1, true, i3d::mat4f(proj));

	glDrawArrays(GL_LINES, 0, m_position.size());

	glBindVertexArray(0);
	glFinish();

	m_position.clear();
	m_color.clear();
}


void PhysicsDebugDrawer::drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &color) {
	m_position.push_back(from.getX());
	m_position.push_back(from.getY());
	m_position.push_back(from.getZ());

	m_color.push_back(color.getX());
	m_color.push_back(color.getY());
	m_color.push_back(color.getZ());

	m_position.push_back(to.getX());
	m_position.push_back(to.getY());
	m_position.push_back(to.getZ());

	m_color.push_back(color.getX());
	m_color.push_back(color.getY());
	m_color.push_back(color.getZ());
}


void PhysicsDebugDrawer::drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor) {
	m_position.push_back(from.getX());
	m_position.push_back(from.getY());
	m_position.push_back(from.getZ());

	m_color.push_back(fromColor.getX());
	m_color.push_back(fromColor.getY());
	m_color.push_back(fromColor.getZ());

	m_position.push_back(to.getX());
	m_position.push_back(to.getY());
	m_position.push_back(to.getZ());

	m_color.push_back(toColor.getX());
	m_color.push_back(toColor.getY());
	m_color.push_back(toColor.getZ());
}


void PhysicsDebugDrawer::drawContactPoint (const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) { }


void PhysicsDebugDrawer::reportErrorWarning (const char *warningString) {
	cout << "BULLLET HAS SOME WIERD ERRORS" << warningString << endl;
}


void PhysicsDebugDrawer::draw3dText (const btVector3 &location, const char *textString) { }


void PhysicsDebugDrawer::setDebugMode (int debugMode) { m_mode = debugMode; }


int PhysicsDebugDrawer::getDebugMode () const {	return m_mode; }








// 
// Physical System
//
static void physicsSystemTickCallback(btDynamicsWorld *world, btScalar timeStep) {
	PhysicsSystem *w = reinterpret_cast<PhysicsSystem *>(world->getWorldUserInfo());
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



	// Debug HACK
	m_debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    dynamicsWorld->setDebugDrawer(&m_debugDrawer);




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


void PhysicsSystem::registerPhysicsUpdatable(PhysicsUpdatable * c) {
	m_physicsUpdatables.insert(c);
}


void PhysicsSystem::deregisterPhysicsUpdatable(PhysicsUpdatable *c) {
	m_physicsUpdatables.erase(c);
}


void PhysicsSystem::registerRigidBody(RigidBody * c) {
	m_rigidbodies.insert(c);
	c->addToDynamicsWorld(dynamicsWorld);
}


void PhysicsSystem::deregisterRigidBody(RigidBody *c) {
	c->removeFromDynamicsWorld();
	m_rigidbodies.erase(c);
}


void PhysicsSystem::registerCollisionCallback(CollisionCallback *c) {
	m_collisionCallbacks[c->entity().get()].insert(c);
}


void PhysicsSystem::deregisterCollisionCallback(CollisionCallback *c) {
	unordered_set<CollisionCallback *> &cset = m_collisionCallbacks[c->entity().get()];
	cset.erase(c);
	if (cset.empty()) {
		m_collisionCallbacks.erase(c->entity().get());
	}
}


void PhysicsSystem::tick() {
	dynamicsWorld->stepSimulation(1 / 60.f, 10);
}


void PhysicsSystem::debugDraw(i3d::mat4d view, i3d::mat4d proj) {
	dynamicsWorld->debugDrawWorld();
	m_debugDrawer.draw(view, proj);
}


void PhysicsSystem::processPhysicsCallback(btScalar timeStep) {

	// Process collisions
	//
	swap(m_currentFrame, m_lastFrame);
	m_currentFrame.clear();

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; ++i) {
		btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject *obA = contactManifold->getBody0();
		const btCollisionObject *obB = contactManifold->getBody1();

		auto collisionPair = make_pair(obA, obB);

		Physical * physicsA = reinterpret_cast<Physical *>(obA->getUserPointer());
		Physical * physicsB = reinterpret_cast<Physical *>(obB->getUserPointer());

		Entity * entityA = physicsA ? physicsA->entity().get() : nullptr;
		Entity * entityB = physicsB ? physicsB->entity().get() : nullptr;

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j<numContacts; ++j) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			// Actual contact
			if (pt.getDistance() < 0.f) {
				m_currentFrame.insert(collisionPair);
				auto it  = m_lastFrame.find(collisionPair);
				if(it != m_lastFrame.end()) {
					//existing collision
					
					if (m_collisionCallbacks.find(entityA) != m_collisionCallbacks.end())
						for (CollisionCallback *callback : m_collisionCallbacks.at(entityA))
							callback->onCollision(physicsB);

					if (m_collisionCallbacks.find(entityB) != m_collisionCallbacks.end())
						for (CollisionCallback *callback : m_collisionCallbacks.at(entityB))
							callback->onCollision(physicsA);

					m_lastFrame.erase(it);
				} else {
					//new collision
					
					if (m_collisionCallbacks.find(entityA) != m_collisionCallbacks.end())
						for (CollisionCallback *callback : m_collisionCallbacks.at(entityA))
							callback->onCollisionEnter(physicsB);

					if (m_collisionCallbacks.find(entityB) != m_collisionCallbacks.end())
						for (CollisionCallback *callback : m_collisionCallbacks.at(entityB))
							callback->onCollisionEnter(physicsA);

				}

				break; //break out of btManifoldPoint iteration
			}
		}
	}

	for (auto collisionExitPair : m_lastFrame) {
		Physical * physicsA = reinterpret_cast<Physical *>(collisionExitPair.first->getUserPointer());
		Physical * physicsB = reinterpret_cast<Physical *>(collisionExitPair.second->getUserPointer());

		Entity * entityA = physicsA ? physicsA->entity().get() : nullptr;
		Entity * entityB = physicsB ? physicsB->entity().get() : nullptr;


		if (m_collisionCallbacks.find(entityA) != m_collisionCallbacks.end())
			for (CollisionCallback *callback : m_collisionCallbacks.at(entityA))
				callback->onCollisionExit(physicsB);

		if (m_collisionCallbacks.find(entityB) != m_collisionCallbacks.end())
			for (CollisionCallback *callback : m_collisionCallbacks.at(entityB))
				callback->onCollisionExit(physicsA);
	}



	// Process physics updates
	//
	for (PhysicsUpdatable *pc : m_physicsUpdatables)
		pc->physicsUpdate();
}
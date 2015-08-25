#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <btBulletDynamicsCommon.h>

#include "Collider.hpp"
#include "ComponentSystem.hpp"


namespace pxljm {


	//
	// Physics Update component
	//
	class PhysicsUpdatable: public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void physicsUpdate() = 0;
	};



	//
	// Physics component
	//
	class Physical : public virtual EntityComponent {
	public:
		virtual void addToDynamicsWorld(btDynamicsWorld *) = 0;
		virtual void removeFromDynamicsWorld() = 0;
	};


	// Rigid Body component
	//
	class RigidBody: public virtual Physical, private btMotionState  {
	public:
		RigidBody();
		RigidBody(collider_ptr);

		virtual void start();
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void addToDynamicsWorld(btDynamicsWorld *);
		virtual void removeFromDynamicsWorld();

		void setCollider(collider_ptr);
		collider_ptr getCollider();
		btRigidBody * getRigidBody();

		// Bullet Physics related methods btMotionState
		virtual void getWorldTransform (btTransform &) const;
		virtual void setWorldTransform (const btTransform &);

	private:

		void regenerateRigidBody();

		// Rigid body stuff
		btScalar m_mass = 1;
		collider_ptr m_collider = nullptr;
		std::unique_ptr<btRigidBody> m_rigidBody;

		// World attached to
		btDynamicsWorld * m_world = nullptr;
	};



	//
	// Physics Collision Callback component
	//
	class CollisionCallback : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void onCollisionEnter(Physical *);
		virtual void onCollision(Physical *);
		virtual void onCollisionExit(Physical *);
	};



	// 
	// Component System for Physcial Components
	// 
	class PhysicsSystem : public ComponentSystem {
	public:
		PhysicsSystem();
		virtual ~PhysicsSystem();

		void registerPhysicsUpdatable(PhysicsUpdatable *);
		void deregisterPhysicsUpdatable(PhysicsUpdatable *);

		void registerRigidBody(RigidBody *);
		void deregisterRigidBody(RigidBody *);

		void registerCollisionCallback(CollisionCallback *);
		void deregisterCollisionCallback(CollisionCallback *);

		void tick();

		void processPhysicsCallback(btScalar);

	private:

		std::unordered_set<PhysicsUpdatable *> m_physicsUpdatables;
		std::unordered_set<RigidBody *> m_rigidbodies;
		std::unordered_map<Entity *, std::unordered_set<CollisionCallback *>> m_collisionCallbacks;

		// internal collision sets
		bool m_collisionsA_isCurrent = true;
		std::unordered_set<std::pair<const btCollisionObject *, const btCollisionObject *>> m_currentFrame;
		std::unordered_set<std::pair<const btCollisionObject *, const btCollisionObject *>> m_lastFrame;

		// physics internals
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;

		btDiscreteDynamicsWorld* dynamicsWorld;

	};


}
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <btBulletDynamicsCommon.h>

#include "Collider.hpp"
#include "ComponentSystem.hpp"


namespace gecom {


	//
	// Physics Update component
	//
	class PhysicsUpdateComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void physicsUpdate() = 0;
	};



	//
	// Physics component
	//
	class PhysicsComponent : public virtual EntityComponent {
	public:
		virtual void addToDynamicsWorld(btDynamicsWorld *) = 0;
		virtual void removeFromDynamicsWorld() = 0;
	};


	// Rigid Body component
	//
	class RigidBody: public virtual PhysicsComponent, private btMotionState  {
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
	class CollisionCallbackComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void onCollisionEnter(PhysicsComponent *);
		virtual void onCollision(PhysicsComponent *);
		virtual void onCollisionExit(PhysicsComponent *);
	};



	// 
	// Component System for Physcial Components
	// 
	class PhysicsSystem : public ComponentSystem {
	public:
		PhysicsSystem();
		virtual ~PhysicsSystem();

		void registerPhysicsUpdateComponent(PhysicsUpdateComponent *);
		void deregisterPhysicsUpdateComponent(PhysicsUpdateComponent *);

		void registerRigidBody(RigidBody *);
		void deregisterRigidBody(RigidBody *);

		void registerCollisionCallbackComponent(CollisionCallbackComponent *);
		void deregisterCollisionCallbackComponent(CollisionCallbackComponent *);

		void tick();

		void processPhysicsCallback(btScalar);

	private:

		std::unordered_set<PhysicsUpdateComponent *> m_physicsUpdatables;
		std::unordered_set<RigidBody *> m_rigidbodies;
		std::unordered_map<Entity *, std::unordered_set<CollisionCallbackComponent *>> m_collisionCallbacks;

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
#pragma once

#include "Scene.hpp"
#include "Initial3D.hpp"
#include "GECom.hpp"

namespace pxljm {


	class BoxMove : public virtual Updatable {
	public:
		BoxMove() { }

		virtual void update() {
			i3d::vec3d pos = entity()->root()->getPosition();
			entity()->root()->setPosition(pos + i3d::vec3d(0.005, 0, 0));
		}
	};


	class SphereBounce : public virtual PhysicsUpdatable {
	private:
		RigidBody * rigidbody = nullptr;

	public:
		SphereBounce() { }

		virtual void start() {
			rigidbody = entity()->getComponent<RigidBody>();
		}

		virtual void physicsUpdate() {
			i3d::vec3d pos = entity()->root()->getPosition();
			if (entity()->root()->getPosition().y() < 0.1) {
				rigidbody->applyImpulse(i3d::vec3d(0,1,1));
			}
		}
	};


	class CollisionCallbackTest  : public virtual CollisionCallback {
	public:
		CollisionCallbackTest() { }
		virtual void onCollisionEnter(Physical *c) {
			std::cout << "Colliding (ENTER) with :: " << c << std::endl;
		} 

		virtual void onCollision(Physical *c) {
			std::cout << "Colliding with :: " << c << std::endl;
		} 

		virtual void onCollisionExit(Physical *c) {
			std::cout << "Colliding (EXIT) with :: " << c << std::endl;
		} 
	};

	class TriggerTest : public virtual TriggerCallback {
	public:
		TriggerTest() { }
		virtual void onTriggerEnter(Physical *c) {
			std::cout << "TRIGGERED WITH (ENTER) with :: " << c << std::endl;
		}
		virtual void onTrigger(Physical *c) {
			std::cout << "TRIGGERED WITH :: " << c << std::endl;
		}

		virtual void onTriggerExit(Physical *c) {
			std::cout << "TRIGGERED WITH (ENTER) with :: " << c << std::endl;
		}

	};
}
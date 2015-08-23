#pragma once

#include "Entity.hpp"
#include "Initial3D.hpp"
#include "GECom.hpp"

namespace gecom {


	class BoxMove : public virtual UpdateComponent {
	public:
		BoxMove() { }

		virtual void update() {
			entity()->root()->position += i3d::vec3d(0.005, 0, 0);
		}
		
	};

	class SphereBounce : public virtual UpdateComponent {
	private:
		RigidBody * rigidbody = nullptr;

	public:
		SphereBounce() { }

		virtual void start() {
			rigidbody = entity()->getComponent<RigidBody>();
		}

		virtual void update() {
			i3d::vec3d pos = entity()->root()->getPosition();
			if (entity()->root()->getPosition().y() < 0.1) {
				rigidbody->m_rigidBody->applyImpulse(btVector3(0,1,0), btVector3(pos.x(), pos.y(), pos.z()));
			}
		}
		
	};
}
#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "GECom.hpp"


namespace gecom {

	class ColliderShape;
	using collider_ptr = std::shared_ptr<ColliderShape>;

	class SphereCollider;
	class BoxCollider;
	using sphere_collider_ptr = std::shared_ptr<SphereCollider>;
	using box_collider_ptr = std::shared_ptr<BoxCollider>;
	
	class ColliderShape : Uncopyable, public std::enable_shared_from_this<ColliderShape> {
	public:
		virtual ~ColliderShape() { }
		virtual btCollisionShape * getCollisionShape() = 0;
	};


	class SphereCollider : public ColliderShape {
	private:
		btSphereShape m_shape;

	public:
		SphereCollider(btScalar s) : m_shape(s) { }
		virtual btCollisionShape * getCollisionShape() { return &m_shape; }
	};


	class BoxCollider : public ColliderShape {
	private:
		btBoxShape m_shape;

	public:
		BoxCollider(btVector3 v) : m_shape(v) { }
		virtual btCollisionShape * getCollisionShape()  { return &m_shape; }
	};
}
#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "GECom.hpp"


namespace pxljm {

	// Super class
	class ColliderShape;
	using collider_ptr = std::shared_ptr<ColliderShape>;

	// Primitive colliders
	class SphereCollider;
	class BoxCollider;
	class CylinderCollider;
	class CapsuleCollider;
	class ConeCollider;
	class MultiSphereCollider;
	using sphere_collider_ptr      = std::shared_ptr<SphereCollider>;
	using box_collider_ptr         = std::shared_ptr<BoxCollider>;
	using cylinder_collider_ptr    = std::shared_ptr<CylinderCollider>;
	using capsule_collider_ptr     = std::shared_ptr<CapsuleCollider>;
	using cone_collider_ptr        = std::shared_ptr<ConeCollider>;
	using multisphere_collider_ptr = std::shared_ptr<MultiSphereCollider>;

	// Complex colliders


	// Compound collider


	// 
	// Primitive colliders
	// 
	class ColliderShape : gecom::Uncopyable, public std::enable_shared_from_this<ColliderShape> {
	public:
		virtual ~ColliderShape() { }
		virtual btCollisionShape * getCollisionShape() = 0;
	};


	class SphereCollider : public ColliderShape {
	private:
		btSphereShape m_shape;

	public:
		SphereCollider(btScalar radius) : m_shape(radius) { }
		virtual btCollisionShape * getCollisionShape() { return &m_shape; }
	};


	class BoxCollider : public ColliderShape {
	private:
		btBoxShape m_shape;

	public:
		BoxCollider(btVector3 boxHalfExtents) : m_shape(boxHalfExtents) { }
		virtual btCollisionShape * getCollisionShape()  { return &m_shape; }
	};


	// height = y-axis
	class CylinderCollider : public ColliderShape {
	private:
		btCylinderShape m_shape;

	public:
		CylinderCollider(btVector3 halfExtents) : m_shape(halfExtents) { }
		virtual btCollisionShape * getCollisionShape()  { return &m_shape; }
	};


	// total height is height+2*radius
	class CapsuleCollider : public ColliderShape {
	private:
		btCapsuleShape m_shape;

	public:
		CapsuleCollider(btScalar radius, btScalar height) : m_shape(radius, height) { }
		virtual btCollisionShape * getCollisionShape()  { return &m_shape; }
	};


	class ConeCollider : public ColliderShape {
	private:
		btConeShape m_shape;

	public:
		ConeCollider(btScalar radius, btScalar height) : m_shape(radius, height) { }
		virtual btCollisionShape * getCollisionShape()  { return &m_shape; }
	};


	class MultiSphereCollider : public ColliderShape {
	private:
		btMultiSphereShape m_shape;

	public:
		MultiSphereCollider(const btVector3 *positions, const btScalar *radi, int numSpheres)
			: m_shape(positions, radi, numSpheres) { }
		virtual btCollisionShape * getCollisionShape()  { return &m_shape; }
	};





	// 
	// Complex ccolliders
	// 





	// 
	// Compound collider
	// 
	// class CompoundCollider : public ColliderShape {
	// private:
	// 	btCompoundShape m_shape;

	// public:
	// 	CompoundCollider() { }
	// 	virtual btConeShape * getCollisionShape()  { return &m_shape; }
	// };


}
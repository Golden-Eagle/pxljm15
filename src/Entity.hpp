#pragma once

#include <algorithm>
#include <memory>
#include <chrono>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

#include <btBulletDynamicsCommon.h>

#include "GECom.hpp"
#include "GLOW.hpp"
#include "Initial3D.hpp"
#include "SimpleShader.hpp"
#include "Geometry.hpp"
#include "Material.hpp"


namespace gecom {

	//
	// Forward decleration
	//

	// Entity
	// 
	class Entity;
	using entity_ptr = std::shared_ptr<Entity>;

	// Components
	// 
	class EntityComponent;

	class UpdateComponent;

	class TransformComponent;
	class EntityTransform;

	class DrawableComponent;
	class MeshDrawable;

	class LightComponent;
	class DirectionalLight;
	class PointLight;
	class SpotLight;

	// Systems
	// 
	class Scene;
	class ComponentSystem;
	class DrawableSystem;
	class LightSystem;


	///////////////////
	//
	// Entity component
	//
	///////////////////
	class EntityComponent {
	public:
		virtual ~EntityComponent();

		bool hasEntity();
		entity_ptr entity() const;
		
	private:
		
		virtual void start(); // After attaching to entity
		virtual void registerWith(Scene &);
		virtual void deregisterWith(Scene &);

		std::weak_ptr<Entity> m_entity;
		friend class Entity;
	};



	//
	// Update component
	//
	class UpdateComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void update() = 0;
		virtual std::chrono::duration<double> updateInterval();
	};



	//
	// Input Update component
	//
	class InputUpdateComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void inputUpdate() = 0;
	};



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
	// Tranform component
	//
	class TransformComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual i3d::mat4d matrix() = 0;

		virtual i3d::vec3d getPosition() const = 0;
		virtual i3d::quatd getRotation() const = 0;

		virtual void setPosition(i3d::vec3d) = 0;
		virtual void setRotation(i3d::quatd) = 0;
	};



	//
	// Entity Tranform component
	//
	class EntityTransform : public TransformComponent {
	public:
		EntityTransform(i3d::vec3d = i3d::vec3d(), i3d::quatd = i3d::quatd());
		EntityTransform(i3d::quatd);

		virtual i3d::mat4d matrix();

		virtual i3d::vec3d getPosition() const;
		virtual i3d::quatd getRotation() const;

		virtual void setPosition(i3d::vec3d);
		virtual void setRotation(i3d::quatd);

		// virtual i3d::mat4d localMatrix();

		i3d::vec3d position;
		i3d::quatd rotation;
	};



	//
	// Drawable component
	//
	class drawcall {
	public:
		virtual ~drawcall();
		virtual void draw() = 0;
		material_ptr material();
		bool operator< (const drawcall& rhs) const;

	protected:
		material_ptr m_mat;
	};

	class DrawableComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual std::vector<drawcall *> getDrawCalls(i3d::mat4d) = 0;

	};


	// Mesh Drawable
	//
	class mesh_drawcall : public drawcall {
	public:
		mesh_drawcall(i3d::mat4d, material_ptr, mesh_ptr);
		virtual void draw();

	private:
		i3d::mat4f m_mv;
		mesh_ptr m_mesh;
	};

	class MeshDrawable :  public DrawableComponent {
	public:
		MeshDrawable(mesh_ptr, material_ptr);
		virtual ~MeshDrawable();

		virtual std::vector<drawcall *> getDrawCalls(i3d::mat4d);

		mesh_ptr mesh;
		material_ptr material;

	private:
		mesh_drawcall m_cachedDrawcall;
	};



	//
	// Physics component
	//
	class PhysicsComponent : public virtual EntityComponent {
	public:
		virtual ~PhysicsComponent();

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void addToDynamicsWorld(btDynamicsWorld *) = 0;
		virtual void removeFromDynamicsWorld() = 0;
	};


	// Rigid Body component
	//
	class RigidBody: public PhysicsComponent, btMotionState  {
	public:
		RigidBody();
		RigidBody(collider_ptr);

		virtual void start();

		virtual void addToDynamicsWorld(btDynamicsWorld *);
		virtual void removeFromDynamicsWorld();

		void setCollider(collider_ptr);
		collider_ptr getCollider();


		// Bullet Physics related methods btMotionState
		virtual void getWorldTransform (btTransform &) const;
		virtual void setWorldTransform (const btTransform &);

	private:

		void regenerateRigidBody();

		//rigid body stuff
		btScalar m_mass = 1;

		std::unique_ptr<btRigidBody> m_rigidBody;

		collider_ptr m_collider = nullptr;
		btDynamicsWorld * m_world = nullptr;
	};



	//
	// Light component
	//
	class LightComponent : public virtual EntityComponent {
	public:
		virtual ~LightComponent();

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	// Directional Light component
	//
	class DirectionalLight : public LightComponent {
	public:
		DirectionalLight();
	};

	// Directional Light component
	//
	class PointLight : public LightComponent {
	public:
		PointLight();
	};

	// Directional Light component
	//
	class SpotLight : public LightComponent {
	public:
		SpotLight();
	};




	/////////
	//
	// Entity
	//
	/////////
	class Entity : Uncopyable, public std::enable_shared_from_this<Entity> {
	private:
		Scene *m_scene = nullptr;
		EntityTransform m_root;
		std::vector<std::unique_ptr<EntityComponent>> m_dynamicComponents;

	protected:
		std::vector<EntityComponent *> m_components;

	public:
		Entity(i3d::vec3d = i3d::vec3d(), i3d::quatd = i3d::quatd());
		Entity(i3d::quatd);
		virtual ~Entity();

		void registerWith(Scene &);
		void deregister();

		template<typename T, typename... Args>
		T * emplaceComponent(Args&&... args)  {
			std::unique_ptr<T> ec = std::make_unique<T>(std::forward<Args>(args)...);
			T *ecp = ec.get();

			m_dynamicComponents.push_back(std::move(ec));
			m_components.push_back(ecp);

			ecp->m_entity = shared_from_this();

			ecp->start();

			if (m_scene)
				ecp->registerWith(*m_scene);

			return ecp;
		}
		void removeComponent(EntityComponent *);


		EntityTransform * root();
		const std::vector<EntityComponent *> & getAllComponents() const;

		//TODO need to make this depth first search?
		template<typename T>
		T * getComponent() const {
			for (EntityComponent * c : m_components)
				if (auto i = std::dynamic_pointer_cast<T>(c))
					return i;
			return nullptr;
		}

		//TODO need to make this depth first search?
		template<typename T>
		std::vector<T *> getComponents() const {
			std::vector<T *> componentList;
			for (EntityComponent * c : m_components)
				if (auto i = std::dynamic_pointer_cast<T>(c))
					componentList.push_back(i);
			return componentList;
		}

	};




	//////////////////////////////
	//
	// Component System base class
	//
	//////////////////////////////
	class ComponentSystem {
	public:
		virtual ~ComponentSystem();
	};


	// 
	// Component System for Update and InputUpdate Components
	// 
	class UpdateSystem : public ComponentSystem {
	public:
		UpdateSystem();

		void registerUpdateComponent(UpdateComponent *);
		void deregisterUpdateComponent(UpdateComponent *);
		void update();

		void registerInputUpdateComponent(InputUpdateComponent *);
		void deregisterInputUpdateComponent(InputUpdateComponent *);
		void inputUpdate();

	private:
		std::unordered_set<UpdateComponent *> m_updatables;
		std::unordered_set<InputUpdateComponent *> m_inputUpdatables;
	};


	// 
	// Component System for Drawable Components
	// 
	class DrawableSystem : public ComponentSystem {
	public:
		DrawableSystem();

		void registerDrawableComponent(DrawableComponent *);
		void deregisterDrawableComponent(DrawableComponent *);

		std::priority_queue<drawcall *> getDrawQueue(i3d::mat4d);

	private:
		std::unordered_set<DrawableComponent *> m_drawables;
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

		void registerPhysicsComponent(PhysicsComponent *);
		void deregisterPhysicsComponent(PhysicsComponent *);

		void tick();

		void processPhysicsCallback(btScalar);

	private:
		std::unordered_set<PhysicsUpdateComponent *> m_physicsUpdatables;
		std::unordered_set<PhysicsComponent *> m_rigidbodies;

		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;

		btDiscreteDynamicsWorld* dynamicsWorld;

	};


	//
	// Component System for Light Components
	//
	class LightSystem : public ComponentSystem {
	public:
		LightSystem();

		void registerLightComponent(LightComponent *);
		void deregisterLightComponent(LightComponent *);

		const std::unordered_set<LightComponent *> & getLights();

	private:
		std::unordered_set<LightComponent *> m_lights;
	};
}
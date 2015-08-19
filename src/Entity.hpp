#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <queue>
#include <vector>

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



	//
	// Entity component
	//
	class EntityComponent {
	public:
		virtual ~EntityComponent();

		virtual void registerWith(Scene &);

		bool hasParent();
		entity_ptr getParent() const;
		
	private:
		std::weak_ptr<Entity> m_parent;
		friend class Entity;
	};



	//
	// Update component
	//
	class UpdateComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;

		virtual void update() = 0;
	};



	//
	// Tranform component
	//
	class TransformComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;

		virtual i3d::mat4d matrix() = 0;
	};



	//
	// Entity Tranform component
	//
	class EntityTransform : public virtual TransformComponent {
	public:
		EntityTransform(i3d::vec3d = i3d::vec3d(), i3d::quatd = i3d::quatd());
		EntityTransform(i3d::quatd);

		virtual i3d::mat4d matrix();
		virtual i3d::mat4d localMatrix();


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

	class MeshDrawable :  public virtual DrawableComponent {
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
	// Light component
	//
	class LightComponent : public virtual EntityComponent {
	public:
		virtual ~LightComponent();

		virtual void registerWith(Scene &) override;
	};

	// Directional Light component
	//
	class DirectionalLight : public virtual LightComponent {
	public:
		DirectionalLight();
	};

	// Directional Light component
	//
	class PointLight : public virtual LightComponent {
	public:
		PointLight();
	};

	// Directional Light component
	//
	class SpotLight : public virtual LightComponent {
	public:
		SpotLight();
	};





	//
	// Entity
	//
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

		template<typename T, typename... Args>
		T * emplaceComponent(Args&&... args)  {
			std::unique_ptr<T> ec = std::make_unique<T>(std::forward<Args>(args)...);
			T *ecp = ec.get();

			m_dynamicComponents.push_back(std::move(ec));
			m_components.push_back(ecp);

			ecp->m_parent = shared_from_this();

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





	//
	// Component System base class
	//
	class ComponentSystem {
	public:
		virtual ~ComponentSystem();
	};


	// 
	// Component System for Drawable Components
	// 
	class DrawableSystem : public ComponentSystem {
	public:
		DrawableSystem();
		virtual ~DrawableSystem();

		void addDrawable(DrawableComponent *);
		void removeDrawable(DrawableComponent *);
		std::priority_queue<drawcall *> getDrawQueue(i3d::mat4d);

	private:
		std::vector<DrawableComponent *> m_drawables;
	};


	//
	// Component System for Light Components
	//
	class LightSystem : public ComponentSystem {
	public:
		LightSystem();
		virtual ~LightSystem();

		void addLight(LightComponent *);
		void removeLight(LightComponent *);
		std::vector<LightComponent *> getLights();

	private:
		std::vector<LightComponent *> m_lights;
	};
}
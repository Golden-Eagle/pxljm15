#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
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

	class Entity;
	using entity_ptr = std::shared_ptr<Entity>;

	class EntityComponent;
	using entity_comp_ptr = std::shared_ptr<EntityComponent>;

	class Drawable;
	class MeshDrawable;
	using entity_draw_ptr = std::shared_ptr<Drawable>;
	using entity_mesh_ptr = std::shared_ptr<MeshDrawable>;

	class Transform;
	class EntityTransform;
	using transform_ptr = std::shared_ptr<Transform>;
	using entity_transform_ptr = std::shared_ptr<EntityTransform>;

	class Light;
	class DirectionalLight;
	class PointLight;
	class SpotLight;
	using light_ptr = std::shared_ptr<Light>;
	using directional_light_ptr = std::shared_ptr<DirectionalLight>;
	using point_light_ptr = std::shared_ptr<PointLight>;
	using spot_light_ptr = std::shared_ptr<SpotLight>;

	class Scene;
	class ComponentSystem;
	class DrawableSystem;
	class LightingSystem;


	//
	// Entity
	//
	class Entity : Uncopyable, public std::enable_shared_from_this<Entity> {
	public:
		Entity(i3d::vec3d = i3d::vec3d(), i3d::quatd = i3d::quatd());
		Entity(i3d::quatd);
		// Entity(entity_ptr, const i3d::vec3d &, const i3d::quatd &);
		// Entity(entity_ptr, const i3d::quatd &);
		~Entity();

		void addComponent( entity_comp_ptr ec );

		entity_transform_ptr root() const;
		const std::vector<entity_comp_ptr> & getComponents() const;

		template<typename T>
		std::vector<std::shared_ptr<T>> getComponent() const;


	private:
		entity_transform_ptr m_root;
		std::vector<entity_comp_ptr> m_components;
	};



	//
	// Entity component
	//
	class EntityComponent : Uncopyable, public std::enable_shared_from_this<EntityComponent> {
	public:

		EntityComponent();
		virtual ~EntityComponent();
		virtual void update(Scene &sc);

		bool hasParent();
		entity_ptr getParent() const;

	protected:
		virtual void registerTo(ComponentSystem *);
		virtual void registerTo(DrawableSystem *);
		virtual void registerTo(LightingSystem *);
		friend class ComponentSystem;
		friend class DrawableSystem;
		friend class LightingSystem;
		
	private:
		std::weak_ptr<Entity> m_parent;
		friend class Entity;
	};



	//
	// Tranform component
	//
	class Transform : public EntityComponent {
	public:
		virtual ~Transform() = 0;
		virtual i3d::mat4d matrix() = 0;
	};



	//
	// Entity Tranform component
	//
	class EntityTransform : public EntityComponent {
	public:
		EntityTransform();
		EntityTransform(i3d::vec3d = i3d::vec3d(), i3d::quatd = i3d::quatd());
		EntityTransform(i3d::quatd);
		virtual ~EntityTransform();
		virtual i3d::mat4d matrix();
		virtual i3d::mat4d localMatrix();

		i3d::vec3d position;
		i3d::quatd rotation;
	};



	//
	// Drawable component
	//
	class Drawable : public EntityComponent {
	public:

		// Draw call data
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

		virtual std::vector<drawcall *> getDrawCalls(i3d::mat4d) = 0;

	protected:
		virtual void registerTo(DrawableSystem *) override final;
	};

	// Mesh Drawable
	//
	class MeshDrawable :  public Drawable {
	public:

		// Mesh draw call data
		//
		class mesh_drawcall : public Drawable::drawcall {
		public:
			mesh_drawcall(i3d::mat4d, material_ptr, mesh_ptr);
			virtual void draw();

		private:
			i3d::mat4f m_mv;
			mesh_ptr m_mesh;
		};

		MeshDrawable(mesh_ptr, material_ptr);

		virtual std::vector<Drawable::drawcall *> getDrawCalls(i3d::mat4d);

		mesh_ptr mesh;
		material_ptr material;
	};



	//
	// Light component
	//
	class Light : public EntityComponent {
	public:
		virtual ~Light();
	protected:
		virtual void registerTo(LightingSystem *);
	};

	// Directional Light component
	//
	class DirectionalLight : public Light {
	public:
		DirectionalLight();
	};

	// Directional Light component
	//
	class PointLight : public Light {
	public:
		PointLight();
	};

	// Directional Light component
	//
	class SpotLight : public Light {
	public:
		SpotLight();
	};



	//
	// Component System base class
	//
	class ComponentSystem {
	public:
		virtual ~ComponentSystem();
		virtual void addComponent(entity_comp_ptr) = 0;
	};


	// 
	// Component System for Drawable Components
	// 
	class DrawableSystem : public ComponentSystem {
	public:
		DrawableSystem();
		virtual ~DrawableSystem();
		virtual void addComponent(entity_comp_ptr) override final;

		void registerDrawable(entity_draw_ptr);
		virtual std::vector<Drawable::drawcall *> getDrawList(i3d::mat4d);

	private:
		std::vector<std::weak_ptr<Drawable>> m_drawables;
	};


	//
	// Component System for Light Components
	//
	class LightingSystem : public ComponentSystem {
	public:
		LightingSystem();
		virtual ~LightingSystem();
		virtual void addComponent(entity_comp_ptr) override final;

		void registerLight(light_ptr);
		virtual std::vector<light_ptr> getLights();

	private:
		std::vector<std::weak_ptr<Light>> m_lights;
	};
}
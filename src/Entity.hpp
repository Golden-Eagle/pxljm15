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
	class Drawable;
	class MeshDrawable;
	class Transform;
	using entity_comp_ptr = std::shared_ptr<EntityComponent>;
	using entity_draw_ptr = std::shared_ptr<Drawable>;
	using entity_mesh_ptr = std::shared_ptr<MeshDrawable>;
	using entity_tran_ptr = std::shared_ptr<Transform>;

	class Scene;
	class ComponentSystem;
	class DrawableSystem;


	//
	// Entity
	//
	class Entity : Uncopyable, public std::enable_shared_from_this<Entity> {
	public:
		Entity();
		~Entity();

		void addComponent( entity_comp_ptr ec );

		entity_tran_ptr root() const;
		const std::vector<entity_comp_ptr> & getComponents() const;

		template<typename T>
		std::vector<std::shared_ptr<T>> getComponent() const;


	private:
		entity_tran_ptr m_root = std::make_shared<Transform>();
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
		friend class ComponentSystem;
		friend class DrawableSystem;
		
	private:
		std::weak_ptr<Entity> m_parent;
		friend class Entity;
	};



	//
	// Tranform component
	//
	class Transform : public EntityComponent {
	public:
		Transform();

		i3d::mat4d matrix();

	private:
		i3d::mat4d m_transform;
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

		MeshDrawable();

		virtual std::vector<Drawable::drawcall *> getDrawCalls(i3d::mat4d);

		mesh_ptr mesh;
		material_ptr material;
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
		std::vector<std::weak_ptr< Drawable >> m_drawables;
	};

}
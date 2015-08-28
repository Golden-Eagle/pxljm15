#pragma once

#include <iostream>
#include <queue>
#include <memory>
#include <vector>

#include "GECom.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "Initial3D.hpp"

// Entity Stuff
//
#include "ComponentSystem.hpp"

#include "Entity.hpp"
#include "DrawSystem.hpp"
#include "LightSystem.hpp"
#include "PhysicsSystem.hpp"
#include "UpdateSystem.hpp"
#include "SoundSystem.hpp"

namespace pxljm {

	class Scene;
	using scene_ptr = std::shared_ptr<Scene>;

	//Colection of entities
	class Scene : public std::enable_shared_from_this<Scene> {
	private:
		std::vector<entity_ptr> m_entities;

		UpdateSystem   m_updateSystem;
		DrawableSystem m_drawableSystem;
		PhysicsSystem  m_physicsSystem;
		LightSystem    m_lightSystem;
		SoundSystem		 m_soundSystem;

		//TEMP TODO needs to be moved into a component
		Projection m_projection;
		FPSCamera m_camera;
		Renderer m_renderer;
		gecom::Window *m_window;

	public:
		Scene(gecom::Window *win);
		~Scene();

		void tick();
		void add( entity_ptr e);

		DrawableSystem & drawableSystem();
		PhysicsSystem & physicsSystem();
		UpdateSystem & updateSystem();
		LightSystem & lightSystem();
	};
}
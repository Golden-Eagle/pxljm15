
#include <iostream>
#include <queue>
#include <vector>

#include "GECom.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "Initial3D.hpp"


namespace gecom {

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

		//TEMP TODO needs to be moved into a component
		Projection m_projection;
		FPSCamera m_camera;
		Renderer m_renderer;
		Window *m_window;

	public:
		Scene(Window *win) : m_camera(win, i3d::vec3d(0, 10, 3)), m_renderer(win), m_window(win) { }


		~Scene() { }


		void tick() {

			// Input Update
			// 
			m_updateSystem.inputUpdate();

			// Physics
			// 
			m_physicsSystem.tick();

			// Update
			// 
			m_camera.update();
			m_updateSystem.update();

			// Animation
			// 

			// Render
			// 
			double zfar = 200.0;
			auto size = m_window->size();
			int w = size.w;
			int h = size.h;

			if (w != 0 && h != 0) {

				m_projection.setPerspectiveProjection(i3d::math::pi() / 3, double(w) / h, 0.1, zfar);
				i3d::mat4d proj_matrix = m_projection.getProjectionTransform();
				i3d::mat4d view_matrix = m_camera.getViewTransform();

				std::priority_queue<drawcall *> drawQueue = m_drawableSystem.getDrawQueue(view_matrix);
				m_renderer.renderScene(proj_matrix, drawQueue);
			}
		}


		void add( entity_ptr e){
			e->registerWith(*this);
			m_entities.push_back(e);
		}


		DrawableSystem & drawableSystem() { return m_drawableSystem; }
		PhysicsSystem & physicsSystem() { return m_physicsSystem; }
		UpdateSystem & updateSystem() { return m_updateSystem; }
		LightSystem & lightSystem() { return m_lightSystem; }
	};
}
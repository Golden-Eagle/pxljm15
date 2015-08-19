
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

		LightSystem *m_lightSystem;
		DrawableSystem *m_drawableSystem;
		PhysicalSystem *m_physicalSystem;

		std::vector<UpdateComponent *> m_updateComponents;

		//TEMP TODO needs to be moved into a component
		Projection *m_projection;
		FPSCamera *m_camera;
		Window *m_window;
		Renderer *m_renderer;

	public:
		Scene(Window *win) : m_window(win) {
			m_window = win;
			m_renderer = new Renderer(win);
			m_lightSystem = new LightSystem();
			m_drawableSystem = new DrawableSystem();
			m_physicalSystem = new PhysicalSystem();

			m_projection = new Projection();
			m_camera = new FPSCamera(win, i3d::vec3d(0, 0, 3));
		}


		~Scene() {
			delete m_renderer;
			delete m_lightSystem;
			delete m_drawableSystem;
			delete m_physicalSystem;
			delete m_projection;
			delete m_camera;
		}

		void tick() {

			//Physics Loop
			// 
				//Bullet update
				// 
				m_physicalSystem->tick();

				//Fixed update
				// 

			//Update
			// 
			m_camera->update();
			for (UpdateComponent * uc : m_updateComponents)
				uc->update();

			//Animation
			// 

			//Render
			// 
			double zfar = 200.0;
			auto size = m_window->size();
			int w = size.w;
			int h = size.h;

			if (w != 0 && h != 0) {

				m_projection->setPerspectiveProjection(i3d::math::pi() / 3, double(w) / h, 0.1, zfar);
				i3d::mat4d proj_matrix = m_projection->getProjectionTransform();
				i3d::mat4d view_matrix = m_camera->getViewTransform();

				std::priority_queue<drawcall *> drawQueue = m_drawableSystem->getDrawQueue(view_matrix);
				m_renderer->renderScene(proj_matrix, drawQueue);
			}
		}


		void add( entity_ptr e){
			e->registerWith(*this);
			for ( EntityComponent *ec : e->getAllComponents() )
				ec->registerWith(*this);

			m_entities.push_back(e);
		}


		// Drawable System
		//
		void registerDrawableComponent(DrawableComponent *ec) { m_drawableSystem->addDrawable(ec); }
		void deregisterDrawableComponent(DrawableComponent *ec) { m_drawableSystem->removeDrawable(ec);	}


		// Update System
		//
		void registerUpdateComponent(UpdateComponent *ec) { m_updateComponents.push_back(ec); }
		void deregisterUpdateComponent(UpdateComponent *ec) {
			for (auto it = m_updateComponents.begin(); it != m_updateComponents.end(); it++)
				if (*it == ec)
					m_updateComponents.erase(it);
		}


		// Physics System
		//
		void registerPhysicalComponent(PhysicalComponent *ec) { m_physicalSystem->addPhysics(ec); }
		void deregisterPhysicalComponent(PhysicalComponent *ec) { m_physicalSystem->removePhysics(ec);	}


		// Light System
		//
		void registerLightComponent(LightComponent *ec) { m_lightSystem->addLight(ec); }
		void deregisterLightComponent(LightComponent *ec) {	m_lightSystem->removeLight(ec);	}
	};
}
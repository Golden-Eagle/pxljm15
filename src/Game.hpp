
#include <vector>
#include <stdexcept>
#include <thread>

#include "Assets.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "GLOW.hpp"
#include "Renderer.hpp"
#include "Initial3D.hpp"
#include "Window.hpp"
#include "SimpleShader.hpp"


namespace gecom {

class Game {
	public:
		Game() {
			win = gecom::createWindow().size(1024, 768).hint(GLFW_SAMPLES, 16).title("Capybara").visible(true);
			win->makeContextCurrent();

			win->onKeyPress.subscribe([&](const gecom::key_event &e) {
				if (e.key == GLFW_KEY_TAB) {
					}

				return false;
			}).forever();


			
			//Renderer creation
			//
			m_renderer = new Renderer(win);

			
			gecom::assets::init("./AssetConfig");


			//
			// HACKY scene creation and population code here
			//
			m_scene = std::make_shared<Scene>();

			// Cube
			//
			entity_ptr cube = std::make_shared<Entity>();
			entity_mesh_ptr md = std::make_shared<MeshDrawable>(
				gecom::assets::getMesh("cube"),
				gecom::assets::getMaterial("basic"));
			cube->addComponent(md);
			m_scene->add(cube);


			// Sphere
			//
			entity_ptr sphere = std::make_shared<Entity>(i3d::vec3d(2, 0, 2));
			md = std::make_shared<MeshDrawable>(
				gecom::assets::getMesh("sphere"),
				gecom::assets::getMaterial("basic"));
			sphere->addComponent(md);
			m_scene->add(sphere);


			// Plane
			//
			entity_ptr plane = std::make_shared<Entity>(i3d::vec3d(0, -5, 0));
			md = std::make_shared<MeshDrawable>(
				gecom::assets::getMesh("plane"),
				gecom::assets::getMaterial("basic"));
			plane->addComponent(md);
			m_scene->add(plane);

		}

		~Game();


		void run() {
			double lastFPSTime = glfwGetTime();
			int fps = 0;

			while (!win->shouldClose()) {
				glfwPollEvents();

				double now = glfwGetTime();
				auto size = win->size();

				// render!
				if (size.w != 0 && size.h != 0) {
					m_renderer->renderScene(m_scene);
				}

				win->swapBuffers();

				if (now - lastFPSTime > 1) {
					char fpsString[200];
					sprintf(
						fpsString, "Skadi [%d FPS @%dx%d]",
						fps, win->width(), win->height());
					win->title(fpsString);
					fps = 0;
					lastFPSTime = now;
				}
				fps++;
			}

			delete win;

			glfwTerminate();
		}
		
	private:
		Window *win;
		scene_ptr m_scene;
		Renderer *m_renderer;
	};
}
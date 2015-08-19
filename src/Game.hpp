
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
#include "ComponentTest.hpp"


namespace gecom {

	class Game {
	public:
		Game() {
			m_win = gecom::createWindow().size(1024, 768).hint(GLFW_SAMPLES, 16).title("Pxljm 2015").visible(true);
			m_win->makeContextCurrent();

			m_win->onKeyPress.subscribe([&](const gecom::key_event &e) {
				if (e.key == GLFW_KEY_TAB) {
					}

				return false;
			}).forever();

			
			gecom::assets::init("./AssetConfig.json");


			//
			// HACKY scene creation and population code here
			//
			m_scene = std::make_shared<Scene>(m_win);

			// Cube
			//
			entity_ptr cube = std::make_shared<Entity>();
			cube->emplaceComponent<MeshDrawable>(
				gecom::assets::getMesh("cube"),
				gecom::assets::getMaterial("basic"));

			cube->emplaceComponent<BoxMove>();
			m_scene->add(cube);


			// Sphere
			//
			entity_ptr sphere = std::make_shared<Entity>(i3d::vec3d(2, 40, 2));
			sphere->emplaceComponent<MeshDrawable>(
				gecom::assets::getMesh("sphere"),
				gecom::assets::getMaterial("basic"));
			m_scene->add(sphere);


			// Plane
			//
			entity_ptr plane = std::make_shared<Entity>(i3d::vec3d(0, -5, 0));
			plane->emplaceComponent<MeshDrawable>(
				gecom::assets::getMesh("plane"),
				gecom::assets::getMaterial("basic"));
			m_scene->add(plane);

		}

		~Game();


		void run() {
			double lastFPSTime = glfwGetTime();
			int fps = 0;

			while (!m_win->shouldClose()) {
				glfwPollEvents();

				double now = glfwGetTime();

				// Game loop
				m_scene->tick();

				m_win->swapBuffers();

				if (now - lastFPSTime > 1) {
					char fpsString[200];
					sprintf(
						fpsString, "Pxljm 2015 [%d FPS @%dx%d]",
						fps, m_win->width(), m_win->height());
					m_win->title(fpsString);
					fps = 0;
					lastFPSTime = now;
				}
				fps++;
			}

			delete m_win;

			glfwTerminate();
		}
		
	private:
		Window *m_win;
		scene_ptr m_scene;
		Renderer *m_renderer;
	};
}

#include <vector>
#include <stdexcept>
#include <thread>

#include "Camera.hpp"
#include "Scene.hpp"
#include "GLOW.hpp"
#include "Renderer.hpp"
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


			//
			// HACKY scene creation and population code here
			//


			m_scene = std::make_shared<Scene>();
			entity_ptr e = std::make_shared<Entity>();
			entity_draw_ptr md = std::make_shared<MeshDrawable>();
			md.mesh = std::make_shared<Mesh>();
			md.material = std::make_shared<Material>();
			e->addComponent(md);
			m_scene->add(e);

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
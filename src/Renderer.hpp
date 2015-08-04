
#include <iostream>
#include <vector>

#include "Camera.hpp"
#include "GECom.hpp"
#include "Entity.hpp"
#include "GLOW.hpp"
#include "Initial3D.hpp"
#include "SimpleShader.hpp"
#include "Window.hpp"


namespace gecom {
	class Renderer {
	public:
		Renderer( Window *win ) : m_win(win) {
			m_projection = new Projection();
			m_camera = new FPSCamera(win, i3d::vec3d(0, 0, 3), 0, 0);
		}
		~Renderer();

		void renderScene(scene_ptr s) {

			glClearColor(1.f, 1.f, 1.f, 1.f); // default background color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			double zfar = 200.0;

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);



			auto size = m_win->size();
			int w = size.w;
			int h = size.h;
			glViewport(0, 0, w, h);


			//
			// Draw forward rendering
			//
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


			m_projection->setPerspectiveProjection(i3d::math::pi() / 3, double(w) / h, 0.1, zfar);
			m_camera->update();

			i3d::mat4d proj_matrix = m_projection->getProjectionTransform();
			i3d::mat4d view_matrix = m_camera->getViewTransform();


			std::vector< entity_draw_ptr > drawList = s->getDrawList();
			for ( auto drawable : drawList ) {
				// Bind shader program
				// Bind material properties
				// Bind Geometry
				// Then render


				material_ptr m = drawable->getMaterial();
				m->shader->bind();
				m->bind();
				drawable->draw(view_matrix, proj_matrix);
			}

			glFinish();
		}

	private:
		Window *m_win;
		Projection *m_projection;
		FPSCamera *m_camera;
	};
}
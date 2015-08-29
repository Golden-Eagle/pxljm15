
#include "Renderer.hpp"

namespace pxljm {

	void Renderer::renderScene(Scene &s) {

		glClearColor(1.f, 1.f, 1.f, 1.f); // default background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);



		auto size = m_win->framebufferSize();
		int w = size.w;
		int h = size.h;

		if (w != 0 && h != 0) {
			s.cameraSystem().update(w, h);
			glViewport(0, 0, w, h);

			//
			// Draw forward rendering
			//
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			i3d::mat4d view = s.cameraSystem().getPrimaryCamera()->getViewMatrix();
			i3d::mat4d proj = s.cameraSystem().getPrimaryCamera()->getProjectionMatrix();
			std::priority_queue<DrawCall *> drawList = s.drawableSystem().getDrawQueue(view);

			for (; !drawList.empty(); drawList.pop()) {
				auto d = drawList.top();
				// Bind shader program
				// Bind material properties
				// Bind Geometry
				// Then render

				material_ptr m = d->material();
				m->shader->bind();
				m->bind(proj);
				d->draw();
			}

			s.physicsSystem().debugDraw(s);
			glFinish();
		}
	}


}

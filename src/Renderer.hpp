#pragma once

#include <iostream>
#include <queue>
#include <vector>

#include "GECom.hpp"
#include "Entity.hpp"
#include "GLOW.hpp"
#include "Initial3D.hpp"
#include "SimpleShader.hpp"
#include "Material.hpp"
#include "Geometry.hpp"
#include "Window.hpp"


namespace gecom {
	class Renderer {
	public:
		Renderer( Window *win ) : m_win(win) { }
		~Renderer() { }

		void renderScene(i3d::mat4d proj, std::priority_queue<drawcall *> drawList) {

			glClearColor(1.f, 1.f, 1.f, 1.f); // default background color
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

			glFinish();
		}

	private:
		Window *m_win;
	};
}
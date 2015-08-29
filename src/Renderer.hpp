#pragma once

#include <iostream>
#include <queue>
#include <vector>

#include <gecom/Window.hpp>
#include <gecom/Initial3D.hpp>

#include "Scene.hpp"


namespace pxljm {
	class Renderer {
	public:
		Renderer(gecom::Window *win) : m_win(win) { }
		~Renderer() { }

		void renderScene(Scene &s);

		void initFBO(gecom::size2i);

	private:
		gecom::Window *m_win;

		gecom::size2i m_fbsize { 0, 0 };
		GLuint m_fbo_scene = 0;
		GLuint m_tex_scene_depth = 0;
		GLuint m_tex_scene_color = 0;
		GLuint m_tex_scene_normal = 0;

	};
}

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

	private:
		gecom::Window *m_win;
	};
}

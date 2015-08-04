#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>

#include "GECom.hpp"
#include "Initial3D.hpp"


namespace gecom {

	class Mesh;

	using mesh_ptr = std::shared_ptr<Mesh>;

	class Mesh {
	public:
		Mesh();
		~Mesh();

		void bind();

		void draw();
	private:
		GLuint m_vao;
	};
}
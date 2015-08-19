#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include <btBulletDynamicsCommon.h>

#include "GECom.hpp"
#include "GLOW.hpp"
#include "Initial3D.hpp"


namespace gecom {

	class Mesh;

	using mesh_ptr = std::shared_ptr<Mesh>;

	class Mesh : Uncopyable, public std::enable_shared_from_this<Mesh> {
	public:
		Mesh(const std::string &);
		~Mesh();

		void bind();

		void draw();

	private:
		void generateGeometry();
		void loadFromObj(const std::string &);

		GLuint m_vao = 0;
		GLuint m_ibo = 0;
		GLuint m_vbo_pos = 0;
		GLuint m_vbo_norm = 0;
		GLuint m_vbo_uv = 0;


		std::vector<float> m_positions;
		std::vector<float> m_normals;
		std::vector<float> m_uvs;
		std::vector<unsigned int> m_triangles;
	};
}
#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>

#include "GECom.hpp"
#include "Initial3D.hpp"
#include "SimpleShader.hpp"


namespace gecom {

	class Material;
	class Shader;

	using material_ptr = std::shared_ptr<Material>;
	using shader_ptr = std::shared_ptr<Shader>;

	class Material : Uncopyable, public std::enable_shared_from_this<Material> {
	public:
		Material();
		~Material();

		void bind();

		shader_ptr shader;
	};

	class Shader : Uncopyable, public std::enable_shared_from_this<Shader> {
	public:
		Shader();
		~Shader();
		void bind();

	private:
		GLuint m_prog;
	};

}
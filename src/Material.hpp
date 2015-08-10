#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <map>

#include "GECom.hpp"
#include "GLOW.hpp"
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

		void bind(i3d::mat4d);

		shader_ptr shader;
	};

	class Shader : Uncopyable, public std::enable_shared_from_this<Shader> {
	public:
		Shader();
		Shader(const std::string &);
		~Shader();
		void bind();

		GLuint uniformLocation(const std::string &);
		GLuint program() const;

	
	private:
		std::map<std::string, GLuint> m_uniformLocationCache;
		GLuint m_prog = 0;
	};

}
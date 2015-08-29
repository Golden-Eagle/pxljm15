#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <map>

#include <gecom/Uncopyable.hpp>
#include <gecom/GL.hpp>
#include <gecom/Initial3D.hpp>

#include "SimpleShader.hpp"


namespace pxljm {

	class Material;
	class Shader;

	using material_ptr = std::shared_ptr<Material>;
	using shader_ptr = std::shared_ptr<Shader>;

	class Material : gecom::Uncopyable, public std::enable_shared_from_this<Material> {
	private:
		bool m_texture;
		i3d::vec3d m_color;
		// Color Texture

		// Normal Texture

	public:
		Material();
		~Material();

		void bind(i3d::mat4d);

		shader_ptr shader;
	};

	class Shader : gecom::Uncopyable, public std::enable_shared_from_this<Shader> {
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
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
		// bool m_texture;
		i3d::vec3d m_color = i3d::vec3d(0.5, 0.5, 0.5);
		// Color Texture


		float m_metalicity;
		float m_roughness;
		float m_specular;
		

		// Normal Texture



	public:
		Material();
		~Material();

		void bind(i3d::mat4d, float);

		shader_ptr shader;
	};

	class Shader : gecom::Uncopyable, public std::enable_shared_from_this<Shader> {
	private:
		std::map<std::string, GLuint> m_uniformLocationCache;
		std::map<GLenum, std::map<std::string, GLuint>> m_subroutineUniformIndexCache;
		std::map<GLenum, std::map<std::string, GLuint>> m_subroutineIndexCache;
		GLuint m_prog = 0;

	public:
		static shader_ptr fromFile(const std::string &);
		Shader(const std::string &);
		~Shader();
		void bind();

		GLuint uniformLocation(const std::string &);
		GLint  activeSubroutines(GLenum shadertype);
		GLuint subroutineUniformIndex(GLenum, const std::string &);
		GLuint subroutineIndex(GLenum, const std::string &);
		GLuint program() const;
	};
}
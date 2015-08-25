#include "Assets.hpp"
#include "Material.hpp"

using namespace std;
using namespace pxljm;


Material::Material() { }


Material::~Material() { }


void Material::bind(i3d::mat4d projectionMatrix) {
	glUniformMatrix4fv(shader->uniformLocation("uProjectionMatrix"), 1, true, i3d::mat4f(projectionMatrix));
}



Shader::Shader() {
	static const char *shader_prog_src = R"delim(
		/*
		 *
		 * Default shader program for writing to scene buffer using GL_TRIANGLES
		 *
		 */

		uniform mat4 uModelViewMatrix;
		uniform mat4 uProjectionMatrix;

		#ifdef _VERTEX_

		layout(location = 0) in vec3 aPosition;
		layout(location = 1) in vec3 aNormal;
		layout(location = 2) in vec3 aUV;

		out VertexData {
			vec4 pos;
			vec4 normal;
			vec2 uv;
		} v_out;

		void main() {
			vec4 p = (uModelViewMatrix * vec4(aPosition, 1.0));
			gl_Position = uProjectionMatrix * p;
			v_out.pos = p;
			v_out.normal = (uModelViewMatrix * vec4(aNormal, 0.0));
		}

		#endif


		#ifdef _FRAGMENT_

		in VertexData {
			vec4 pos;
			vec4 normal;
			vec2 uv;
		} v_in;

		out vec3 color;

		void main() {
			vec3 grey = vec3(0.8, 0.8, 0.8);
		    color = abs(v_in.normal.z) * grey;
		}

		#endif
		)delim";

	cout << "NOOOOP" << endl; 

	m_prog = gecom::makeShaderProgram("410 core", { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, shader_prog_src);
}


Shader::Shader(const string &filename) {
	ifstream fileStream(filename);

	if (fileStream) {
		stringstream buffer;
		buffer << fileStream.rdbuf();
		m_prog = gecom::makeShaderProgram("410 core", { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, buffer.str());
	}
}


Shader::~Shader() { }


void Shader::bind() {
	glUseProgram(m_prog);
}


GLuint Shader::uniformLocation(const string &name) {
	try {
		return m_uniformLocationCache.at(name);
	} catch (const std::out_of_range& oor) {
		GLuint location = glGetUniformLocation(m_prog, name.c_str());
		if (location == GLuint(-1)) 
			throw runtime_error("Error :: Tried to access invalid uniform name.");
		m_uniformLocationCache[name] = location;
		return location;
	}
}


GLuint Shader::program() const {
	return m_prog;
}
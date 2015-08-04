#include "Material.hpp"

using namespace std;
using namespace gecom;


Material::Material() { 
	shader = make_shared<Shader>();
}


Material::~Material() { }


void Material::bind() { }



Shader::Shader() {
	static const char *shader_prog_src = R"delim(
		/*
		 *
		 * Default shader program for writing to scene buffer using GL_TRIANGLES
		 *
		 */

		uniform mat4 modelViewMatrix;
		uniform mat4 projectionMatrix;

		#ifdef _VERTEX_

		layout(location = 0) in vec3 vertexPosition_modelspace;
		layout(location = 1) in vec3 vertexColor;

		out vec3 fragmentColor;

		void main() {
			vec3 pos_v = (modelViewMatrix * vec4(vertexPosition_modelspace, 1.0)).xyz;
			gl_Position = projectionMatrix * vec4(pos_v, 1.0);
			fragmentColor = vertexColor;
		}

		#endif


		#ifdef _FRAGMENT_

		in vec3 fragmentColor;
		out vec3 color;

		void main(){
		    color = fragmentColor;
		}

		#endif
		)delim";
	if (prog == 0) {
		prog = makeShaderProgram("330 core", { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, shader_prog_src);
	}
}


Shader::~Shader() { }


void Shader::bind() {
	glUseProgram(prog);
}

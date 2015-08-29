#include "Assets.hpp"
#include "Material.hpp"

using namespace std;
using namespace pxljm;


Material::Material() {}


Material::~Material() { }


void Material::bind(i3d::mat4d projectionMatrix, float zfar) {
	// basic
	glUniformMatrix4fv(shader->uniformLocation("uProjectionMatrix"), 1, true, i3d::mat4f(projectionMatrix));
	glUniform1f(shader->uniformLocation("uZFar"), zfar);

	// material
	glUniform3f(shader->uniformLocation("uColor"), m_color.x(), m_color.y(), m_color.z());
	glUniform1f(shader->uniformLocation("uMetalicity"), 0);
	glUniform1f(shader->uniformLocation("uRoughness"), 0);
	glUniform1f(shader->uniformLocation("uSpecular"), 1.0);


	vector<GLuint> fragSubroutines(shader->activeSubroutines(GL_FRAGMENT_SHADER), 0);

	if (fragSubroutines.size() > 0) {
		auto addFragSubroutine = [&](const string &uniform, const string &name) {
			GLuint uidx = shader->subroutineUniformIndex(GL_FRAGMENT_SHADER, uniform);
			GLuint pidx = shader->subroutineIndex(GL_FRAGMENT_SHADER, name);
			if (uidx != GL_INVALID_INDEX && pidx != GL_INVALID_INDEX) {
				fragSubroutines[uidx] = pidx;
			}
		};

		addFragSubroutine("getColor", "colorFromValue");
		addFragSubroutine("getNormal", "normalFromValue");

		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, fragSubroutines.size(), &fragSubroutines[0]);
	}
}


shader_ptr Shader::fromFile(const std::string &filename) {
	ifstream fileStream(filename);

	if (fileStream) {
		stringstream buffer;
		buffer << fileStream.rdbuf();
		return make_shared<Shader>(buffer.str());
	}
}


Shader::Shader(const string &text) {
	m_prog = gecom::makeShaderProgram("410 core", { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, text);
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
		m_uniformLocationCache[name] = location;
		return location;
	}
}


GLint Shader::activeSubroutines(GLenum shadertype) {
	GLint activeCount = 0;
	glGetProgramStageiv(m_prog, shadertype, GL_ACTIVE_SUBROUTINES, &activeCount);
	return activeCount;
}


GLuint Shader::subroutineUniformIndex(GLenum shadertype, const string &name) {
	try {
		return m_subroutineUniformIndexCache.at(shadertype).at(name);
	}
	catch (const std::out_of_range& oor) {
		GLuint index = glGetSubroutineUniformLocation(m_prog, shadertype, name.c_str());
		m_subroutineUniformIndexCache[shadertype][name] = index;
		return index;
	}
}


GLuint Shader::subroutineIndex(GLenum shadertype, const string &name) {
	try {
		return m_subroutineIndexCache.at(shadertype).at(name);
	}
	catch (const std::out_of_range& oor) {
		GLuint index = glGetSubroutineIndex(m_prog, shadertype, name.c_str());
		m_subroutineIndexCache[shadertype][name] = index;
		return index;
	}
}


GLuint Shader::program() const {
	return m_prog;
}
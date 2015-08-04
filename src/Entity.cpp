#include "Entity.hpp"

using namespace std;
using namespace gecom;

//
// Entity
//
Entity::Entity() { m_components.push_back(m_root); }


Entity::~Entity() {}


void Entity::addComponent( entity_comp_ptr ec ) {
	// when optimized scene
	// check to see if in current scene
	// if so, register component with current scene

	// add to entity
	auto it = find(m_components.begin(), m_components.end(), ec);
	if (it == m_components.end()) 
		m_components.push_back(ec);

	ec->m_parent = shared_from_this();
}


entity_tran_ptr Entity::root() const {
	return m_root;
}


const vector<entity_comp_ptr> & Entity::getComponents() const {
	return m_components;
}


template<typename T>
vector<shared_ptr<T>> Entity::getComponent() const {
	vector<T> cl;
	for (entity_comp_ptr c : m_components)
		if (auto i = dynamic_pointer_cast<T>(c))
			cl.push_back(i);
	return cl;
}



//
// Entity component
//
EntityComponent::EntityComponent() { }


EntityComponent::~EntityComponent() { }


void EntityComponent::registerTo(ComponentSystem *cs) { }


void EntityComponent::registerTo(DrawableSystem *ds) { }


void EntityComponent::update(Scene &s) { }


bool EntityComponent::hasParent() { return bool(m_parent.lock()); }


entity_ptr EntityComponent::getParent() const { return m_parent.lock(); }



//
// Tranform component
//
i3d::mat4d Transform::matrix() {
	return m_transform;
}



//
// Drawable component
//
void Drawable::registerTo(DrawableSystem *ds) {
	ds->registerDrawable(static_pointer_cast<Drawable>(shared_from_this()));
}


void Drawable::loadShader() {
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
	glUseProgram(prog);
}


void Drawable::loadMaterial() {

}


void Drawable::loadData() {
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// vertex positions
		float pos[] = {
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

		GLuint vbo_pos;
		glGenBuffers(1, &vbo_pos);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
		glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), pos, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		float col[] = {
			0.583f, 0.771f, 0.014f,
			0.609f, 0.115f, 0.436f,
			0.327f, 0.483f, 0.844f,
			0.822f, 0.569f, 0.201f,
			0.435f, 0.602f, 0.223f,
			0.310f, 0.747f, 0.185f,
			0.597f, 0.770f, 0.761f,
			0.559f, 0.436f, 0.730f,
			0.359f, 0.583f, 0.152f,
			0.483f, 0.596f, 0.789f,
			0.559f, 0.861f, 0.639f,
			0.195f, 0.548f, 0.859f,
			0.014f, 0.184f, 0.576f,
			0.771f, 0.328f, 0.970f,
			0.406f, 0.615f, 0.116f,
			0.676f, 0.977f, 0.133f,
			0.971f, 0.572f, 0.833f,
			0.140f, 0.616f, 0.489f,
			0.997f, 0.513f, 0.064f,
			0.945f, 0.719f, 0.592f,
			0.543f, 0.021f, 0.978f,
			0.279f, 0.317f, 0.505f,
			0.167f, 0.620f, 0.077f,
			0.347f, 0.857f, 0.137f,
			0.055f, 0.953f, 0.042f,
			0.714f, 0.505f, 0.345f,
			0.783f, 0.290f, 0.734f,
			0.722f, 0.645f, 0.174f,
			0.302f, 0.455f, 0.848f,
			0.225f, 0.587f, 0.040f,
			0.517f, 0.713f, 0.338f,
			0.053f, 0.959f, 0.120f,
			0.393f, 0.621f, 0.362f,
			0.673f, 0.211f, 0.457f,
			0.820f, 0.883f, 0.371f,
			0.982f, 0.099f, 0.879f
		};

		GLuint vbo_col;
		glGenBuffers(1, &vbo_col);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_col);
		glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), col, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	glBindVertexArray(vao);
}


void Drawable::draw(i3d::mat4f worldViewMat, i3d::mat4f projMat) {
	glUniformMatrix4fv(glGetUniformLocation(prog, "projectionMatrix"), 1, true, i3d::mat4f(projMat));
	glUniformMatrix4fv(glGetUniformLocation(prog, "modelViewMatrix"), 1, true, worldViewMat * getParent()->root()->matrix());

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}



// 
// ComponentSystem
// 
ComponentSystem::~ComponentSystem() {}



// 
// DrawableSystem
// 
DrawableSystem::DrawableSystem() {}


DrawableSystem::~DrawableSystem() {}


void DrawableSystem::addComponent(entity_comp_ptr c){
	c->registerTo(static_cast<DrawableSystem *>(this));
}


void DrawableSystem::registerDrawable(entity_draw_ptr d) {
	m_drawables.push_back(d);
	cout << "registered!" << endl;
}


vector< entity_draw_ptr > DrawableSystem::getDrawList() {
	vector< entity_draw_ptr > drawList;
	for (auto it = m_drawables.begin() ; it != m_drawables.end(); ) {
		if (auto drawable = (*it).lock()) {
			drawList.push_back(drawable);
			++it;
		} else {
			it = m_drawables.erase(it);
		}
	}
	return drawList;
}
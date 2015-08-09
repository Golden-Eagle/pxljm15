#include "Geometry.hpp"

using namespace std;
using namespace gecom;



Mesh::Mesh(const std::string &filepath) {
	loadFromObj(filepath);





	// setup the buffer!
	if (m_vao == 0) {
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_ibo);
		glGenBuffers(1, &m_vbo_pos);
		glGenBuffers(1, &m_vbo_norm);
		glGenBuffers(1, &m_vbo_uv);



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_uv);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Fill in the geometry
		generateGeometry();
	}
}


Mesh::~Mesh() { }


void Mesh::generateGeometry() {

	// vertex positions
	uint i[] = {
		0, 1, 2,
		3, 2, 1 
	};

	// vertex positions
	float p[] = {
		-1.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,
		 1.0f,  1.0f, -0.0f
	};

	// vertex normals
	float n[] = {
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f
	};

	// vertex texture
	float t[] = {
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f
	};

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i), i, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(n), n, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t), t, GL_STATIC_DRAW);

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Mesh::bind() {
	glBindVertexArray(m_vao);
}


void Mesh::draw() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


void Mesh::loadFromObj(const string &filepath) {
	cout << "Load obj from " << filepath << endl;
}
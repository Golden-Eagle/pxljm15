#include "Mesh.hpp"

using namespace std;
using namespace pxljm;
using namespace i3d;

// @josh fix this shit
using uint = unsigned;

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


Mesh::~Mesh() {
	//TODO delete buffers
}


void Mesh::generateGeometry() {
	// glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * m_triangles.size(), &m_triangles[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_positions.size(), &m_positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_norm);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_normals.size(), &m_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_uvs.size(), &m_uvs[0], GL_STATIC_DRAW);

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);
}


void Mesh::bind() {
	glBindVertexArray(m_vao);
}


void Mesh::draw() {
	glDrawElements(GL_TRIANGLES, m_triangles.size(), GL_UNSIGNED_INT, nullptr);
}


void Mesh::loadFromObj(const string &filepath) {
	cout << "Load obj from " << filepath << endl;

	// Make sure our geometry information is cleared
	vector<float> positions;
	vector<float> normals;
	vector<float> uvs;


	ifstream objFile(filepath);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filepath << endl;
		throw runtime_error("Error :: could not open file.");
	}

	// cout << "Reading file " << filepath << endl;
	
	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		if (!objLine.fail()) {

			if (mode == "v") {
				float x, y, z;
				objLine >> x >> y >> z;
				positions.push_back(x);
				positions.push_back(y);
				positions.push_back(z);

			} else if(mode == "vn") {
				float x, y, z;
				objLine >> x >> y >> z;
				normals.push_back(x);
				normals.push_back(y);
				normals.push_back(z);

			} else if(mode == "vt") {
				float u, v;
				objLine >> u >> v;
				uvs.push_back(u);
				uvs.push_back(v);

			} else if(mode == "f") {

				int i = 0;
				while (objLine.good() && i++<3){
					int pi,ti, ni;
					m_triangles.push_back(m_positions.size()/3);

					objLine >> pi; // position index
					--pi;
					m_positions.push_back(positions[pi*3]);
					m_positions.push_back(positions[pi*3+1]);
					m_positions.push_back(positions[pi*3+2]);

					if (objLine.peek() == '/') {	// Look ahead for a match
						objLine.ignore(1);			// Ignore the '/' character

						if (objLine.peek() != '/') {
							objLine >> ti; // uv index
							--ti;
							m_uvs.push_back(uvs[ti*2]);
							m_uvs.push_back(uvs[ti*2+1]);
						}

						if (objLine.peek() == '/') {
							objLine.ignore(1);
							objLine >> ni; // normal index
							--ni;
							m_normals.push_back(normals[ni*3]);
							m_normals.push_back(normals[ni*3+1]);
							m_normals.push_back(normals[ni*3+2]);
						}
					}
				}
			}
		}
	}

	// TODO update with new i3d at some point?
	// cout << "Reading OBJ file is DONE." << endl;
	// cout << m_positions.size()/3 << " positions" << endl;
	// cout << m_uvs.size()/2 << " uv coords" << endl;
	// cout << m_normals.size()/3 << " normals" << endl;
	// cout << m_triangles.size()/3 << " faces" << endl;

}
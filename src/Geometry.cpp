#include "Geometry.hpp"

using namespace std;
using namespace gecom;
using namespace i3d;



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
	unsigned int i[] = {
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

	// Make sure our geometry information is cleared
	vector<vec3f> positions;
	vector<vec3f> normals;
	vector<float> uvs;


	ifstream objFile(filepath);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filepath << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filepath << endl;
	
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
				positions.push_back(vec3f(x, y, z));

			} else if(mode == "vn") {
				float x, y, z;
				objLine >> x >> y >> z;
				normals.push_back(vec3f(x, y, z));

			} else if(mode == "vt") {
				float u, v;
				objLine >> u >> v;
				uvs.push_back(u);
				uvs.push_back(v);

			} else if(mode == "f") {

				int i = 0;
				while (objLine.good() && i++<3){
					int pi,ti, ni;
					m_triangles.push_back(m_positions.size());

					objLine >> pi; // position index
					--pi;
					m_positions.push_back(positions[pi]);

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
							m_normals.push_back(normals[ni]);
						}
					}
				}
			}
		}
	}

	// TODO update with new i3d at some point?
	cout << "Reading OBJ file is DONE." << endl;
	cout << m_positions.size() << " positions" << endl;
	cout << m_uvs.size()/2 << " uv coords" << endl;
	cout << m_normals.size() << " normals" << endl;
	cout << m_triangles.size()/3 << " faces" << endl;


	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) {
		cout << "You you even have normals bro?" << endl;

		// // Create the normals as 3d vectors of 0
		// for (size_t i = 1; i < m_points.size(); i++) {
		// 	m_normals.push_back(vec3());
		// }

		// // Add the normal for every face to each vertex-normal
		// for (size_t i = 0; i < m_triangles.size(); i++) {
		// 	vertex &a = m_triangles[i].v[0];
		// 	vertex &b = m_triangles[i].v[1];
		// 	vertex &c = m_triangles[i].v[2];

		// 	a.n = a.p;
		// 	b.n = b.p;
		// 	c.n = c.p;

		// 	vec3 ab = m_points[b.p] - m_points[a.p];
		// 	vec3 ac = m_points[c.p] - m_points[a.p];

		// 	vec3 norm = cross(ab, ac);
		// 	if (length(norm) > 0) {
		// 		m_normals[a.n] += normalize(norm);
		// 		m_normals[b.n] += normalize(norm);
		// 		m_normals[c.n] += normalize(norm);
		// 	}
		// }

		// // Normalize the normals
		// for (size_t i = 1; i < m_points.size(); i++) {
		// 	m_normals[i] = normalize(m_normals[i]);
		// }
	}
}


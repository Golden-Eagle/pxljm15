#pragma once

#include <algorithm>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include <btBulletDynamicsCommon.h>

#include "GECom.hpp"
#include "GLOW.hpp"
#include "Entity.hpp"
#include "Material.hpp"
#include "Initial3D.hpp"

#include "DrawSystem.hpp"


//TODO put mesh stuff in here?
namespace gecom {

	class Mesh;
	using mesh_ptr = std::shared_ptr<Mesh>;

	class Mesh : Uncopyable, public std::enable_shared_from_this<Mesh> {
	public:
		Mesh(const std::string &);
		~Mesh();

		void bind();

		void draw();

	private:
		void generateGeometry();
		void loadFromObj(const std::string &);

		GLuint m_vao = 0;
		GLuint m_ibo = 0;
		GLuint m_vbo_pos = 0;
		GLuint m_vbo_norm = 0;
		GLuint m_vbo_uv = 0;


		std::vector<float> m_positions;
		std::vector<float> m_normals;
		std::vector<float> m_uvs;
		std::vector<unsigned int> m_triangles;
	};



	// Mesh Drawable
	//
	class mesh_drawcall : public drawcall {
	private:
		i3d::mat4f m_mv;
		mesh_ptr m_mesh;
	public:
		//TODO clean up
		mesh_drawcall(i3d::mat4d mv, material_ptr mat, mesh_ptr mesh)
			: m_mv(mv), m_mesh(mesh) { m_mat = mat; }
		virtual void draw() {
			m_mesh->bind();
			glUniformMatrix4fv(m_mat->shader->uniformLocation("uModelViewMatrix"), 1, true, m_mv);
			m_mesh->draw();
		}
	};

	class MeshDrawable :  public virtual DrawableComponent {
	private:
		mesh_drawcall m_cachedDrawcall;

	public:
		mesh_ptr mesh;
		material_ptr material;

		MeshDrawable(mesh_ptr m, material_ptr mat)
			: mesh(m), material(mat), m_cachedDrawcall(i3d::mat4d(), mat, m) {  }

		virtual std::vector<drawcall *> getDrawCalls(i3d::mat4d view) {
			std::vector<drawcall *> drawcallList;
			m_cachedDrawcall = mesh_drawcall(view * entity()->root()->matrix(), material, mesh);
			drawcallList.push_back(&m_cachedDrawcall);
			return drawcallList;
		}
	};
}
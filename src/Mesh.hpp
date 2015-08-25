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
namespace pxljm {

	class Mesh;
	using mesh_ptr = std::shared_ptr<Mesh>;

	class Mesh : gecom::Uncopyable, public std::enable_shared_from_this<Mesh> {
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
	class MeshDrawCall : public DrawCall {
	private:
		i3d::mat4f m_mv;
		mesh_ptr m_mesh;
	public:
		//TODO clean up
		MeshDrawCall(i3d::mat4d mv, material_ptr mat, mesh_ptr mesh)
			: m_mv(mv), m_mesh(mesh) { m_mat = mat; }
		virtual void draw() {
			m_mesh->bind();
			glUniformMatrix4fv(m_mat->shader->uniformLocation("uModelViewMatrix"), 1, true, m_mv);
			m_mesh->draw();
		}
	};

	class MeshDrawable :  public virtual DrawableComponent {
	private:
		MeshDrawCall m_cachedDrawCall;

	public:
		mesh_ptr mesh;
		material_ptr material;

		MeshDrawable(mesh_ptr m, material_ptr mat)
			: m_cachedDrawCall(i3d::mat4d(), mat, m), mesh(m), material(mat) {  }

		virtual std::vector<DrawCall *> getDrawCalls(i3d::mat4d view) {
			std::vector<DrawCall *> drawCallList;
			m_cachedDrawCall = MeshDrawCall(view * entity()->root()->matrix(), material, mesh);
			drawCallList.push_back(&m_cachedDrawCall);
			return drawCallList;
		}
	};
}
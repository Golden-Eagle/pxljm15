#pragma once

#include <string>
#include <map>

#include "Material.hpp"
#include "Geometry.hpp"

namespace gecom {
	namespace assets {
		void init(const std::string &);
		material_ptr getMaterial(const std::string &);
		shader_ptr getShader(const std::string &);
		mesh_ptr getMesh(const std::string &);
	}
}

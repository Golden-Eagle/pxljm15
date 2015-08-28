#pragma once

#include <string>
#include <map>
#include <memory>

#include "json.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Image.hpp"

namespace pxljm {
	namespace assets {
		void init(const std::string &);
		material_ptr getMaterial(const std::string &);
		shader_ptr getShader(const std::string &);
		mesh_ptr getMesh(const std::string &);
    image_ptr getImage(const std::string &);
	}
}

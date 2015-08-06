#include "Assets.hpp"

using namespace std;


namespace gecom {

	namespace {
		map<string, material_ptr> materialMap;
		map<string, shader_ptr> shaderMap;
		map<string, mesh_ptr> meshMap;
	}

	namespace assets {
		void init(const std::string &configPath) {
			// TODO parse here
		}


		material_ptr getMaterial(const std::string &tag) {
			return materialMap[tag];
		}


		shader_ptr getShader(const std::string &tag) {
			return shaderMap[tag];
		}


		mesh_ptr getMesh(const std::string &tag) {
			return meshMap[tag];
		}
	}
}
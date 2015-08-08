#include "Assets.hpp"

using namespace std;
using json = nlohmann::json;

namespace gecom {

	namespace {
		map<string, material_ptr> materialMap;
		map<string, shader_ptr> shaderMap;
		map<string, mesh_ptr> meshMap;
	}

	namespace assets {
		void init(const std::string &configPath) {
			cout << "readfing file" << endl;
			ifstream configFile(configPath);
			if (configFile) {
				json config;
				config << configFile;
				//TODO fail case

				cout << " >> meshes" << endl;
				for (auto it = config["meshes"].begin(); it != config["meshes"].end(); ++it)
					cout << " " << it.key() << ":" << it.value() << endl;
					// meshMap[it.key()] = make_shared(Mesh(it.value()));

				cout << " >> materials" << endl;
				for (auto it = config["materials"].begin(); it != config["materials"].end(); ++it)
					cout << " " << it.key() << ":" << it.value() << endl;
					// meshMap[it.key()] = make_shared(Mesh(it.value()));
				
				cout << " >> shaders" << endl;
				for (auto it = config["shaders"].begin(); it != config["shaders"].end(); ++it)
					cout << " " << it.key() << ":" << it.value() << endl;
					// meshMap[it.key()] = make_shared(Mesh(it.value()));

			}
			cout << "finsihed readfing file" << endl;

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
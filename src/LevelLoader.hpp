
#ifndef PXLJM_LEVELLOADER_HPP
#define PXLJM_LEVELLOADER_HPP

#include <iostream>
#include "json.hpp"
#include "Scene.hpp"

using namespace nlohmann;

namespace pxljm {
  class LevelLoader {
  public:
    void Load(std::shared_ptr<Scene> scene, std::string levelname) {
      std::ifstream t("res/levels/" + levelname);
      std::stringstream buffer;
      buffer << t.rdbuf();

      auto j = json::parse(buffer.str());

      for(auto waypoint : j["waypoints"]) {
        double x = waypoint["x"];
        double y = waypoint["y"];
        double z = waypoint["z"];

        auto waypt = std::make_shared<Entity>(i3d::vec3d(x, y, z));
        waypt->emplaceComponent<MeshDrawable>(
          assets::getMesh("waypoint"),
          assets::getMaterial("basic")
        );
		waypt->emplaceComponent<Trigger>(std::make_shared<SphereCollider>(1));

        scene->add(waypt);
      }

      for(auto asteroid : j["asteroids"]) {
        double x = asteroid["x"];
        double y = asteroid["y"];
        double z = asteroid["z"];

        auto aster = std::make_shared<Entity>(i3d::vec3d(x, y, z));
		aster->emplaceComponent<MeshDrawable>(
          assets::getMesh("cube"),
          assets::getMaterial("basic")
        );

        scene->add(aster);
      }
    }
  };
}


#endif

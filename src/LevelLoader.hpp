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
          assets::getMesh("cube"),
          assets::getMaterial("basic")
        );

        scene->add(waypt);
      }

      for(auto waypoint : j["asteroids"]) {
        double x = waypoint["x"];
        double y = waypoint["y"];
        double z = waypoint["z"];

        auto waypt = std::make_shared<Entity>(i3d::vec3d(x, y, z));
        waypt->emplaceComponent<MeshDrawable>(
          assets::getMesh("cube"),
          assets::getMaterial("basic")
        );

        scene->add(waypt);
      }
    }
  };
}

#ifndef PXLJM_LEVELLOADER_HPP
#define PXLJM_LEVELLOADER_HPP

#include <iostream>
#include "json.hpp"
#include "Scene.hpp"

using namespace nlohmann;

namespace pxljm {
    class PlayerScoreComponent : public UIRenderComponent {
    int m_points = 0;
  public:
    void markWaypointComplete() {
      m_points += 1;
    }

    void draw() override {
      ImGui::Begin("Game");

      char msg[1024];
      sprintf(msg, "Score: %d", m_points);

      ImGui::Text(msg);

      ImGui::End();
    }
  };

  class PlayerTriggerCallback : public TriggerCallback {
    PlayerScoreComponent *m_playerScore = nullptr;
  public:
    void onTriggerEnter(Physical *p) override {
      gecom::Log::info() << "trigger fired";

      if(!m_playerScore)
        m_playerScore = p->entity()->getComponent<PlayerScoreComponent>();

      m_playerScore->markWaypointComplete();
    }
  };

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
          assets::getMesh("waypoint_ring"),
          assets::getMaterial("waypoint_ring")
        );
        waypt->emplaceComponent<MeshDrawable>(
          assets::getMesh("waypoint_entrance"),
          assets::getMaterial("waypoint_entrance")
        );
        waypt->emplaceComponent<MeshDrawable>(
          assets::getMesh("waypoint_entrance_slider"),
          assets::getMaterial("waypoint_entrance_slider")
        );
        waypt->emplaceComponent<MeshDrawable>(
          assets::getMesh("waypoint_rock"),
          assets::getMaterial("waypoint_rock")
        );
        waypt->emplaceComponent<MeshDrawable>(
          assets::getMesh("waypoint_rod"),
          assets::getMaterial("waypoint_rod")
        );
        waypt->emplaceComponent<MeshDrawable>(
          assets::getMesh("waypoint_slider"),
          assets::getMaterial("waypoint_slider")
        );

		    waypt->emplaceComponent<Trigger>(std::make_shared<SphereCollider>(150));
        // waypt->emplaceComponent<PlayerTriggerCallback>();

        scene->add(waypt);
      }

      for(auto asteroid : j["asteroids"]) {
        double x = asteroid["x"];
        double y = asteroid["y"];
        double z = asteroid["z"];

        auto aster = std::make_shared<Entity>(i3d::vec3d(x, y, z));
		    aster->emplaceComponent<MeshDrawable>(
          assets::getMesh("asteroid"),
          assets::getMaterial("basic")
        );
        aster->emplaceComponent<RigidBody>(std::make_shared<SphereCollider>(100), 20);

        scene->add(aster);
      }
    }
  };
}


#endif

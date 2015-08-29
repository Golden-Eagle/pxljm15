#ifndef PXLJM_PLAYSTATE_HPP
#define PXLJM_PLAYSTATE_HPP

// fuck these spaces

#include "Pxljm.hpp"
#include "Game.hpp"

namespace pxljm {
	class PlayerControllable : public virtual InputUpdatable {
		void inputUpdate() {
		}
	};

  class PlayState : public State < std::string > {
    std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Entity> m_player;
    Game* m_game;
	gecom::subscription_ptr m_window_scene_sub;

  public:
    PlayState(Game* game) : m_game(game) {
      m_scene = std::make_shared<Scene>(game->window());

			m_player = std::make_shared<Entity>(i3d::vec3d(0, 90, 5));
			m_player->emplaceComponent<MeshDrawable>(
        assets::getMesh("cube"),
        assets::getMaterial("basic"));

			m_player->emplaceComponent<PlayerControllable>();

			m_player->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(pxljm::i3d2bt(i3d::vec3d::one())));
			m_scene->add(m_player);
    }

    virtual action_ptr updateForeground() override {
      // Game loop
      m_scene->update();
      return nullAction();
    }

    virtual void drawForeground() override {
      m_scene->render();
    }
  };
}

#endif

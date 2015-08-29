#ifndef PXLJM_PLAYSTATE_HPP
#define PXLJM_PLAYSTATE_HPP

// fuck these spaces

#include "Pxljm.hpp"
#include "Game.hpp"

namespace pxljm {
	class PlayerControllable : public virtual InputUpdatable {
		virtual void inputUpdate(gecom::WindowEventProxy &wep) override {
			if(wep.getKey(GLFW_KEY_UP)) {
				gecom::Log::info() << "up";
				entity()->getComponent<RigidBody>()->applyImpulse(i3d::vec3d(0, 0, -0.1));
			}
			else if(wep.getKey(GLFW_KEY_DOWN)) {
				gecom::Log::info() << "down";
				entity()->getComponent<RigidBody>()->applyImpulse(i3d::vec3d(0, 0, 0.1));
			}
		}
	};

	class PlayState : public State < std::string > {
		Renderer m_renderer;
		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Entity> m_player;
		Game* m_game;
		gecom::subscription_ptr m_window_scene_sub;


	public:
		PlayState(Game* game) : m_renderer(game->window()), m_game(game) {
			m_scene = std::make_shared<Scene>(game->window());

			m_window_scene_sub = game->window()->subscribeEventDispatcher(m_scene->updateSystem().eventProxy());

			m_player = std::make_shared<Entity>(i3d::vec3d(0, 0, -3));
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
			m_renderer.renderScene(*m_scene);
		}
	};
}

#endif

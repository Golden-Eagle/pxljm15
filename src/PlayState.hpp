#ifndef PXLJM_PLAYSTATE_HPP
#define PXLJM_PLAYSTATE_HPP

// fuck these spaces

#include "Pxljm.hpp"
#include "Game.hpp"

namespace pxljm {
	class PlayerControllable : public virtual InputUpdatable {
		RigidBody* m_rigidBody;

		virtual void inputUpdate(gecom::WindowEventProxy &wep) override {
			if(!m_rigidBody) {
				m_rigidBody = entity()->getComponent<RigidBody>();
			}

			if(wep.getKey(GLFW_KEY_UP)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyImpulse(i3d::vec3d(0, 0, -0.1));
			}
			else if(wep.getKey(GLFW_KEY_DOWN)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyImpulse(i3d::vec3d(0, 0, 0.1));
			}
		}
	};

	class CameraControllable : public virtual InputUpdatable {
		RigidBody* m_rigidBody;

		virtual void inputUpdate(gecom::WindowEventProxy &wep) override {
			if(!m_rigidBody) {
				m_rigidBody = entity()->getComponent<RigidBody>();
			}

			gecom::Log::info() << entity()->root()->getPosition();


			if(wep.getKey(GLFW_KEY_W)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyImpulse(i3d::vec3d(0, 0, -0.1));
			}
			else if(wep.getKey(GLFW_KEY_S)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyImpulse(i3d::vec3d(0, 0, 0.1));
			}
		}
	};


	class PlayState : public State < std::string > {
		Renderer m_renderer;
		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Entity> m_player1;
		std::shared_ptr<Entity> m_player2;
		std::shared_ptr<Entity> m_camera;
		PerspectiveCamera *m_cameraComponent;

		Game* m_game;
		gecom::subscription_ptr m_window_scene_sub;


	public:
		PlayState(Game* game) : m_renderer(game->window()), m_game(game) {
			m_scene = std::make_shared<Scene>(game->window());

			m_window_scene_sub = game->window()->subscribeEventDispatcher(m_scene->updateSystem().eventProxy());

			m_player1 = std::make_shared<Entity>(i3d::vec3d(-1, 0, -3));
			m_player1->emplaceComponent<MeshDrawable>(

				assets::getMesh("cube"),
				assets::getMaterial("basic"));

			m_player1->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(pxljm::i3d2bt(i3d::vec3d::one())));
			m_player1->emplaceComponent<PlayerControllable>();
			m_scene->add(m_player1);

			m_player2 = std::make_shared<Entity>(i3d::vec3d(1, 0, -3));
			m_player2->emplaceComponent<MeshDrawable>(

				assets::getMesh("cube"),
				assets::getMaterial("basic"));

			m_player2->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(pxljm::i3d2bt(i3d::vec3d::one())));
			m_scene->add(m_player2);

			m_camera = std::make_shared<Entity>(i3d::vec3d(0, 0, 5));
			m_camera->emplaceComponent<PerspectiveCamera>();
			m_camera->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(pxljm::i3d2bt(i3d::vec3d::one())));
			m_camera->emplaceComponent<CameraControllable>();
			m_cameraComponent = m_camera->getComponent<PerspectiveCamera>();
			m_cameraComponent->registerWith(*m_scene);
			m_scene->cameraSystem().setCamera(m_cameraComponent);

			m_scene->add(m_camera);
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

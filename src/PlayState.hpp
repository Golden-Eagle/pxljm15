#ifndef PXLJM_PLAYSTATE_HPP
#define PXLJM_PLAYSTATE_HPP

// fuck these spaces

#include "Pxljm.hpp"
#include "Game.hpp"
#include <gecom/Window.hpp>

namespace pxljm {
	class PlayerControllable : public virtual InputUpdatable {
		RigidBody* m_rigidBody;

		virtual void inputUpdate(gecom::WindowEventProxy &wep) override {
			if(!m_rigidBody) {
				m_rigidBody = entity()->getComponent<RigidBody>();
			}

			const float keyBoardTorqueScale = 0.05;
			const float keyBoardThrustScale = 0.025;

			if(wep.getKey(GLFW_KEY_UP)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyTorque(keyBoardTorqueScale * i3d::vec3d(-1, 0, 0));
			}
			else if(wep.getKey(GLFW_KEY_DOWN)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyTorque(keyBoardTorqueScale * i3d::vec3d(1, 0, 0));
			}

			if(wep.getKey(GLFW_KEY_LEFT)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyTorque(keyBoardTorqueScale * i3d::vec3d(0, 0, -1));
			}
			else if(wep.getKey(GLFW_KEY_RIGHT)) {
				m_rigidBody->wakeUp();
				m_rigidBody->applyTorque(keyBoardTorqueScale * i3d::vec3d(0, 0, 1));
			}

			if(wep.getKey(GLFW_KEY_SPACE)) {
				auto facing = entity()->root()->getRotation() * i3d::vec3d(0, 0, 1);
				m_rigidBody->wakeUp();
				m_rigidBody->applyImpulse(keyBoardThrustScale * facing);
			}

			if(glfwJoystickPresent(GLFW_JOYSTICK_1)) {
				int count;
				const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

				gecom::Log::info() << "using joy1: " << axes[3] << " : " << axes[4];
				m_rigidBody->wakeUp();
				auto up = entity()->root()->getRotation() * i3d::vec3d(-axes[1] * 0.2, -axes[2] * 0.2, axes[0] * 0.2);
				m_rigidBody->applyTorque(i3d::vec3d(up));
				// for(int i = 0; i < count; i++) {
				// 	gecom::Log::info() << "axis[" << i << "]: " << axes[i];
				// }
				float thrustAmount = ((1 - ((axes[3]+1)/2.0))) * 0.1;
				gecom::Log::info() << "thrusting: " << thrustAmount;
				auto facing = entity()->root()->getRotation() * i3d::vec3d(0, 0, thrustAmount);
				m_rigidBody->wakeUp();
				m_rigidBody->applyImpulse(facing);

			}
		}
	};

	class CameraControllable : public virtual InputUpdatable {
		RigidBody* m_rigidBody;

		virtual void inputUpdate(gecom::WindowEventProxy &wep) override {
			if(!m_rigidBody) {
				m_rigidBody = entity()->getComponent<RigidBody>();
			}

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
		std::shared_ptr<Entity> m_player;
		std::shared_ptr<Entity> m_camera;
		PerspectiveCamera *m_cameraComponent;

		Game* m_game;
		gecom::subscription_ptr m_window_scene_sub;


	public:
		PlayState(Game* game) : m_renderer(game->window()), m_game(game) {
			m_scene = std::make_shared<Scene>(game->window());

			LevelLoader ll;
			ll.Load(m_scene, "sample.json");

			m_window_scene_sub = game->window()->subscribeEventDispatcher(m_scene->updateSystem().eventProxy());

			m_player = std::make_shared<Entity>(i3d::vec3d(0, 0, 20));
			m_player->emplaceComponent<MeshDrawable>(
				assets::getMesh("ship"),
				assets::getMaterial("basic"));

			gecom::Log::info() << "glfw says joy1 is: " << glfwJoystickPresent(GLFW_JOYSTICK_1);

			m_player->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(pxljm::i3d2bt(i3d::vec3d::one())));
			m_player->emplaceComponent<PlayerControllable>();
			m_scene->add(m_player);

			m_camera = std::make_shared<Entity>(i3d::vec3d(0, 0, 0), i3d::quatd::axisangle(i3d::vec3d(0, 1, 0), 3.1415));
			m_camera->emplaceComponent<PerspectiveCamera>();
			m_camera->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(pxljm::i3d2bt(i3d::vec3d::zero())));
			m_camera->emplaceComponent<CameraControllable>();
			m_cameraComponent = m_camera->getComponent<PerspectiveCamera>();
			m_cameraComponent->registerWith(*m_scene);
			m_scene->cameraSystem().setCamera(m_cameraComponent);

			m_scene->add(m_camera);
			m_player->root()->addChild(m_camera->root());
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

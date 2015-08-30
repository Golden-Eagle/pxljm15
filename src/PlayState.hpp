#ifndef PXLJM_PLAYSTATE_HPP
#define PXLJM_PLAYSTATE_HPP

// fuck these spaces

#include "Pxljm.hpp"
#include "Game.hpp"
#include "Joystick.hpp"
#include <gecom/Window.hpp>

namespace pxljm {
	class PlayerControllable : public virtual InputUpdatable {
		RigidBody* m_rigidBody = nullptr;
		JoystickManager m_joystickManager;
		std::shared_ptr<Joystick> m_joystick;

	public:
		PlayerControllable() : m_joystickManager("input.json") {
			m_joystick = m_joystickManager.findJoystick();
		}
		virtual void inputUpdate(gecom::WindowEventProxy &wep) override {
			if(!m_rigidBody) {
				m_rigidBody = entity()->getComponent<RigidBody>();
			}

			float pitchInc = m_joystick->getButtonValue("pitchUp", wep);
			float pitchDec = m_joystick->getButtonValue("pitchDown", wep);
			float pitchDelta = pitchInc + pitchDec;

			float rollInc = m_joystick->getButtonValue("rollLeft", wep);
			float rollDec = m_joystick->getButtonValue("rollRight", wep);
			float rollDelta = rollInc + rollDec;

			float yawInc = m_joystick->getButtonValue("yawLeft", wep);
			float yawDec = m_joystick->getButtonValue("yawRight", wep);
			float yawDelta = yawInc + yawDec;

			float pitch = m_joystick->getAxisValue("pitch") + pitchDelta;
			float roll  = m_joystick->getAxisValue("roll") + rollDelta;
			float yaw   = m_joystick->getAxisValue("yaw") + yawDelta;

			auto up = entity()->root()->getRotation() * i3d::vec3d(pitch, yaw, roll);
			m_rigidBody->applyTorqueImpulse(up);

			float thrust = m_joystick->getAxisValue("thrust");

			auto facing = entity()->root()->getRotation() * i3d::vec3d(0, 0, thrust);
			m_rigidBody->applyImpulse(facing);
		}
	};

	class CameraControllable : public virtual InputUpdatable {
		RigidBody* m_rigidBody;

		virtual void inputUpdate(gecom::WindowEventProxy &wep) override {
			if(!m_rigidBody) {
				m_rigidBody = entity()->getComponent<RigidBody>();
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
			m_player->getComponent<RigidBody>()->setDamping(0.1, 0.9);
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

			auto ui = make_shared<TestUIComponent>();
			m_scene->uiRenderSystem().registerUiComponent(ui);
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

#pragma once

#include <unordered_set>
#include <chrono>
#include <memory>

#include <gecom/Window.hpp>

#include "ComponentSystem.hpp"

namespace pxljm {

	//
	// Update component
	//
	class Updatable : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void update() = 0;
		virtual std::chrono::duration<double> updateInterval();
	};


	//
	// Input Update component
	//
	class InputUpdatable : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void inputUpdate(gecom::WindowEventProxy &) = 0;
	};


	// 
	// Component System for Update and InputUpdate Components
	// 
	class UpdateSystem : public ComponentSystem {
	public:
		UpdateSystem();

		void registerUpdatable(Updatable *);
		void deregisterUpdatable(Updatable *);
		void update();

		void registerInputUpdatable(InputUpdatable *);
		void deregisterInputUpdatable(InputUpdatable *);
		void inputUpdate();

		const std::shared_ptr<gecom::WindowEventProxy> & eventProxy() {
			return m_wep;
		}

	private:
		std::unordered_set<Updatable *> m_updatables;
		std::unordered_set<InputUpdatable *> m_inputUpdatables;
		std::shared_ptr<gecom::WindowEventProxy> m_wep = std::make_shared<gecom::WindowEventProxy>();
	};

}

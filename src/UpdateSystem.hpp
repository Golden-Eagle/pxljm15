#pragma once

#include <unordered_set>
#include <chrono>

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

		virtual void inputUpdate() = 0;
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

	private:
		std::unordered_set<Updatable *> m_updatables;
		std::unordered_set<InputUpdatable *> m_inputUpdatables;
	};

}
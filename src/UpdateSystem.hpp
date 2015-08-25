#pragma once

#include <unordered_set>
#include <chrono>

#include "ComponentSystem.hpp"

namespace gecom {

	//
	// Update component
	//
	class UpdateComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual void update() = 0;
		virtual std::chrono::duration<double> updateInterval();
	};


	//
	// Input Update component
	//
	class InputUpdateComponent : public virtual EntityComponent {
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

		void registerUpdateComponent(UpdateComponent *);
		void deregisterUpdateComponent(UpdateComponent *);
		void update();

		void registerInputUpdateComponent(InputUpdateComponent *);
		void deregisterInputUpdateComponent(InputUpdateComponent *);
		void inputUpdate();

	private:
		std::unordered_set<UpdateComponent *> m_updatables;
		std::unordered_set<InputUpdateComponent *> m_inputUpdatables;
	};

}
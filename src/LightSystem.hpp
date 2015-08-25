#pragma once

#include <unordered_set>

#include "ComponentSystem.hpp"


namespace gecom {
	//
	// Light component
	//
	class LightComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	// Directional Light component
	//
	class DirectionalLight : public virtual LightComponent {
	public:
		DirectionalLight();
	};

	// Directional Light component
	//
	class PointLight : public virtual LightComponent {
	public:
		PointLight();
	};

	// Directional Light component
	//
	class SpotLight : public virtual LightComponent {
	public:
		SpotLight();
	};



	//
	// Component System for Light Components
	//
	class LightSystem : public ComponentSystem {
	public:
		LightSystem();

		void registerLightComponent(LightComponent *);
		void deregisterLightComponent(LightComponent *);

		const std::unordered_set<LightComponent *> & getLights();

	private:
		std::unordered_set<LightComponent *> m_lights;
	};
}
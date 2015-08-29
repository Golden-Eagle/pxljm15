#pragma once

#include <unordered_set>

#include "ComponentSystem.hpp"


namespace pxljm {
	//
	// Light component
	//
	class Light : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	// Directional Light component
	//
	class DirectionalLight : public virtual Light {
	public:
		DirectionalLight();
	};

	// Point Light component
	//
	class PointLight : public virtual Light {
	public:
		PointLight();
	};

	// Spot Light component
	//
	class SpotLight : public virtual Light {
	public:
		SpotLight();
	};



	//
	// Component System for Light Components
	//
	class LightSystem : public ComponentSystem {
	public:
		LightSystem();

		void registerLight(Light *);
		void deregisterLight(Light *);

		const std::unordered_set<Light *> & getLights();

	private:
		std::unordered_set<Light *> m_lights;
	};
}
#pragma once

#include <unordered_set>

#include "ComponentSystem.hpp"


namespace pxljm {
	
	// Directional Light component
	//
	class DirectionalLight : public virtual EntityComponent {
	public:
		DirectionalLight();

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	// Point Light component
	//
	class PointLight : public virtual EntityComponent {
	public:
		PointLight();

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	// Spot Light component
	//
	class SpotLight : public virtual EntityComponent {
	public:
		SpotLight();

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};



	//
	// Component System for Light Components
	//
	class LightSystem : public ComponentSystem {
	public:
		LightSystem();

		void registerDirectionalLight(DirectionalLight *);
		void registerPointLight(PointLight *);
		void registerSpotLight(SpotLight *);

		void deregisterDirectionalLight(DirectionalLight *);
		void deregisterPointLight(PointLight *);
		void deregisterSpotLight(SpotLight *);

		const std::unordered_set<DirectionalLight *> & getDirectionalLights();
		const std::unordered_set<PointLight *> & getPointLights();
		const std::unordered_set<SpotLight *> & getSpotLights();

	private:
		std::unordered_set<DirectionalLight *> m_dirlights;
		std::unordered_set<PointLight *> m_pointlights;
		std::unordered_set<SpotLight *> m_spotlights;
	};
}
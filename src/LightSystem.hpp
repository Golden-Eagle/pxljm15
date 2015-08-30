#pragma once

#include <unordered_set>

#include "ComponentSystem.hpp"


namespace pxljm {
	
	// Directional Light component
	//
	class DirectionalLight : public virtual EntityComponent {
	public:
		DirectionalLight();

		virtual i3d::vec3f radiance() = 0;

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	class DefaultDirectionalLight : public DirectionalLight {
	private:
		i3d::vec3f m_radiance { 0, 0, 0 };

	public:
		virtual i3d::vec3f radiance() override {
			return m_radiance;
		}

		void radiance(const i3d::vec3f &v) {
			m_radiance = v;
		}
	};

	// Point Light component
	//
	class PointLight : public virtual EntityComponent {
	public:
		PointLight();

		virtual i3d::vec3f flux() = 0;
		virtual float radius() = 0;

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	class DefaultPointLight : public PointLight {
	private:
		i3d::vec3f m_flux { 0, 0, 0 };
		float m_radius = 0.1f;

	public:
		virtual i3d::vec3f flux() override {
			return m_flux;
		}

		void flux(const i3d::vec3f v) {
			m_flux = v;
		}

		virtual float radius() override {
			return m_radius;
		}

		void radius(float f) {
			m_radius = f;
		}
	};


	// Spot Light component
	//
	class SpotLight : public virtual EntityComponent {
	public:
		SpotLight();

		virtual i3d::vec3f flux() = 0;
		virtual float radius() = 0;
		virtual float cosCutoff() = 0;
		virtual float exponent() = 0;

		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;
	};

	class DefaultSpotLight : public SpotLight {
	private:
		i3d::vec3f m_flux { 0, 0, 0 };
		float m_radius = 0.1f;
		float m_cos_cutoff = 0.f;
		float m_exponent = 0.f;

	public:
		virtual i3d::vec3f flux() override {
			return m_flux;
		}

		void flux(const i3d::vec3f v) {
			m_flux = v;
		}

		virtual float radius() override {
			return m_radius;
		}

		void radius(float f) {
			m_radius = f;
		}

		virtual float cosCutoff() override {
			return m_cos_cutoff;
		}

		void cosCutoff(float f) {
			m_cos_cutoff = f;
		}

		virtual float exponent() override {
			return m_exponent;
		}

		void exponent(float f) {
			m_exponent = f;
		}
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
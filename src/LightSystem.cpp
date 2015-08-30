
#include "LightSystem.hpp"
#include "Scene.hpp"


using namespace std;
using namespace pxljm;
using namespace i3d;


// Directional Light
//
DirectionalLight::DirectionalLight() { }

void DirectionalLight::registerWith(Scene &s) {
	s.lightSystem().registerDirectionalLight(this);
}

void DirectionalLight::deregisterWith(Scene &s) {
	s.lightSystem().deregisterDirectionalLight(this);
}

// Point Light
//
PointLight::PointLight() { }

void PointLight::registerWith(Scene &s) {
	s.lightSystem().registerPointLight(this);
}

void PointLight::deregisterWith(Scene &s) {
	s.lightSystem().deregisterPointLight(this);
}

// Spot Light
//
SpotLight::SpotLight() { }

void SpotLight::registerWith(Scene &s) {
	s.lightSystem().registerSpotLight(this);
}

void SpotLight::deregisterWith(Scene &s) {
	s.lightSystem().deregisterSpotLight(this);
}


// 
// Lighting System
// 
LightSystem::LightSystem() { }


void LightSystem::registerDirectionalLight(DirectionalLight *l) {
	m_dirlights.insert(l);
}

void LightSystem::registerPointLight(PointLight *l) {
	m_pointlights.insert(l);
}

void LightSystem::registerSpotLight(SpotLight *l) {
	m_spotlights.insert(l);
}

void LightSystem::deregisterDirectionalLight(DirectionalLight *l) {
	m_dirlights.erase(l);
}

void LightSystem::deregisterPointLight(PointLight *l) {
	m_pointlights.erase(l);
}

void LightSystem::deregisterSpotLight(SpotLight *l) {
	m_spotlights.erase(l);
}

const std::unordered_set<DirectionalLight *> & LightSystem::getDirectionalLights() {
	return m_dirlights;
}

const std::unordered_set<PointLight *> & LightSystem::getPointLights() {
	return m_pointlights;
}

const std::unordered_set<SpotLight *> & LightSystem::getSpotLights() {
	return m_spotlights;
}



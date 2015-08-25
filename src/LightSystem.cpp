
#include "LightSystem.hpp"
#include "Scene.hpp"


using namespace std;
using namespace gecom;
using namespace i3d;


// Light component
//
void LightComponent::registerWith(Scene &s) { s.lightSystem().registerLightComponent(this); }


void LightComponent::deregisterWith(Scene &s) { s.lightSystem().deregisterLightComponent(this); }



// Directional Light
//
DirectionalLight::DirectionalLight() { }



// Point Light
//
PointLight::PointLight() { }



// Spot Light
//
SpotLight::SpotLight() { }




// 
// Lighting System
// 
LightSystem::LightSystem() { }


void LightSystem::registerLightComponent(LightComponent *c) {
	m_lights.insert(c);
}


void LightSystem::deregisterLightComponent(LightComponent *c) {
	m_lights.erase(c);
}


const unordered_set<LightComponent *> & LightSystem::getLights() {
	return m_lights;
}
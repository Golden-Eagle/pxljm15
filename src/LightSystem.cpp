
#include "LightSystem.hpp"
#include "Scene.hpp"


using namespace std;
using namespace pxljm;
using namespace i3d;


// Light component
//
void Light::registerWith(Scene &s) { s.lightSystem().registerLight(this); }


void Light::deregisterWith(Scene &s) { s.lightSystem().deregisterLight(this); }



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


void LightSystem::registerLight(Light *c) {
	m_lights.insert(c);
}


void LightSystem::deregisterLight(Light *c) {
	m_lights.erase(c);
}


const unordered_set<Light *> & LightSystem::getLights() {
	return m_lights;
}
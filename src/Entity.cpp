/*
 * Entity.cpp is for declaring the methods required for double dispatch
 * removing the dependency loop problems in C++. All other methods are
 * declared in header for reference.
 */

#include <memory>

#include "Entity.hpp"
#include "Scene.hpp"

using namespace gecom;

void EntityComponent::registerToScene( Scene &sc ) {
	sc.registerComponent( std::static_pointer_cast<EntityComponent>(shared_from_this()) );
}

void DrawableComponent::registerToScene( Scene &sc ) {
	sc.registerComponent( std::static_pointer_cast<DrawableComponent>(shared_from_this()) );
}

// void PhysicsComponent::registerToScene( Scene &sc ) {
// 	sc.registerComponent( std::static_pointer_cast<PhysicsComponent>(shared_from_this()) );
// }
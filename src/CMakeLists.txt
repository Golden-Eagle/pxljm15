

# list headers here
set(headers

	"Pxljm.hpp"

	"json.hpp"

	"Game.hpp"
	"Camera.hpp"
	"Renderer.hpp"
	"ComponentTest.hpp"
	"SimpleShader.hpp"
	
	# Assets
	"Assets.hpp"
	"Material.hpp"
	"Mesh.hpp"

	# Entity
	"ComponentSystem.hpp"
	"Entity.hpp"
	"DrawSystem.hpp"
	"LightSystem.hpp"
	"PhysicsSystem.hpp"
	"UpdateSystem.hpp"

	"Scene.hpp"

	"LevelLoader.hpp"
)


# list sources here
set(sources
	"main.cpp"
	
	"Assets.cpp"
	"Material.cpp"
	"Mesh.cpp"

	"ComponentSystem.cpp"
	"Entity.cpp"
	"DrawSystem.cpp"
	"LightSystem.cpp"
	"PhysicsSystem.cpp"
	"UpdateSystem.cpp"

	"Scene.cpp"
)


add_executable(pxljm ${headers} ${sources})

target_link_libraries(pxljm PRIVATE gecom)

target_link_libraries(pxljm PRIVATE BulletCollision BulletDynamics BulletSoftBody)




























# list headers here
SET(gecom_hdr

	#System headers
	"GECom.hpp"
	"GLOW.hpp"
	"Concurrent.hpp"
	"Initial3D.hpp"
	"Image.hpp"
	"Log.hpp"
	"Perlin.hpp"
	"json.hpp"
	"Window.hpp"

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
SET(gecom_src
	"main.cpp"
	"Log.cpp"
	"Perlin.hpp"
	"Window.cpp"
	"Concurrent.cpp"
	
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


add_executable(gecom ${gecom_hdr} ${gecom_src})

target_link_libraries(gecom glfw ${GLFW_LIBRARIES})

add_definitions(${GLAER_DEFINITIONS})
target_link_libraries(gecom glaer ${GLAER_LIBRARIES})

target_link_libraries(gecom BulletCollision BulletDynamics BulletSoftBody)

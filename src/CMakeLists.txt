# list headers here
SET(gecom_hdr
	"Camera.hpp"
	"GECom.hpp"
	"GLOW.hpp"
	"Initial3D.hpp"
	"Image.hpp"
	"Log.hpp"
	"Perlin.hpp"
	"Window.hpp"
	"Concurrent.hpp"
	"SimpleShader.hpp"
	"ComponentTest.hpp"
	"Scene.hpp"
	"Entity.hpp"
	"Game.hpp"
	"Renderer.hpp"
	"Geometry.hpp"
	"Material.hpp"
	"Assets.hpp"
	"json.hpp"
)


# list sources here
SET(gecom_src
	"main.cpp"
	"Log.cpp"
	"Perlin.hpp"
	"Window.cpp"
	"Concurrent.cpp"
	"Entity.cpp"
	"Geometry.cpp"
	"Material.cpp"
	"Assets.cpp"
)


add_executable(gecom ${gecom_hdr} ${gecom_src})

target_link_libraries(gecom glfw ${GLFW_LIBRARIES})

add_definitions(${GLAER_DEFINITIONS})
target_link_libraries(gecom glaer ${GLAER_LIBRARIES})

target_link_libraries(gecom BulletCollision BulletDynamics BulletSoftBody)

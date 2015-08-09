cmake_minimum_required(VERSION 2.4.3)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

PROJECT(BULLET_PHYSICS)
SET(BULLET_VERSION 2.83)

IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)


IF (NOT CMAKE_BUILD_TYPE)
# SET(CMAKE_BUILD_TYPE "Debug")
 SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE) 

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
#MESSAGE("CMAKE_CXX_FLAGS_DEBUG="+${CMAKE_CXX_FLAGS_DEBUG})

OPTION(USE_DOUBLE_PRECISION "Use double precision"	OFF)
   
IF(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)


#Maya Dynamica plugin is moved to http://dynamica.googlecode.com

SUBDIRS(src)

IF("${CMAKE_GENERATOR}" MATCHES "Unix Makefiles")
	OPTION(INSTALL_LIBS "Set when you want to install libraries" ON)
ELSE()
	IF(APPLE AND FRAMEWORK)
		OPTION(INSTALL_LIBS "Set when you want to install libraries" ON)
	ELSE()
#by default, don't enable the 'INSTALL' option for Xcode and MSVC projectfiles
		OPTION(INSTALL_LIBS "Set when you want to install libraries" OFF)
	ENDIF()
ENDIF()

IF(INSTALL_LIBS)
	SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
	SET (LIB_DESTINATION "lib${LIB_SUFFIX}" CACHE STRING "Library directory name")
	## the following are directories where stuff will be installed to
	SET(INCLUDE_INSTALL_DIR      "include/bullet/" CACHE PATH "The subdirectory to the header prefix")
	SET(PKGCONFIG_INSTALL_PREFIX "lib${LIB_SUFFIX}/pkgconfig/" CACHE STRING "Base directory for pkgconfig files")
	IF(NOT MSVC)
	  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/bullet.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/bullet.pc @ONLY)
  	INSTALL(
	    FILES
    	${CMAKE_CURRENT_BINARY_DIR}/bullet.pc
    	DESTINATION
    	${PKGCONFIG_INSTALL_PREFIX})
	ENDIF(NOT MSVC)
ENDIF(INSTALL_LIBS)

#INSTALL of other files requires CMake 2.6
IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
	OPTION(INSTALL_EXTRA_LIBS "Set when you want extra libraries installed" OFF)
ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)


set (BULLET_CONFIG_CMAKE_PATH lib${LIB_SUFFIX}/cmake/bullet )
list (APPEND BULLET_LIBRARIES LinearMath)
list (APPEND BULLET_LIBRARIES BulletCollision)
list (APPEND BULLET_LIBRARIES BulletDynamics)
list (APPEND BULLET_LIBRARIES BulletSoftBody)
set (BULLET_USE_FILE ${BULLET_CONFIG_CMAKE_PATH}/UseBullet.cmake)
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/BulletConfig.cmake.in
                 ${CMAKE_CURRENT_BINARY_DIR}/BulletConfig.cmake
                 @ONLY ESCAPE_QUOTES
               )
install ( FILES ${CMAKE_CURRENT_SOURCE_DIR}/UseBullet.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/BulletConfig.cmake
          DESTINATION ${BULLET_CONFIG_CMAKE_PATH}
        )

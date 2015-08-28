add_Library(fmod_lowlevel SHARED IMPORTED GLOBAL)

if(UNIX AND APPLE)
  set_target_properties(fmod_lowlevel PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/ext/fmod/api/lowlevel/lib/libfmod.dylib")
elseif(WIN32)
  set_target_properties(fmod_lowlevel PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/ext/fmod/api/lowlevel/lib/libfmod.fmod64")
endif()

set_target_properties(fmod_lowlevel PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/fmod/api/lowlevel/inc")

add_Library(fmod_studio SHARED IMPORTED GLOBAL)

if(UNIX AND APPLE)
  set_target_properties(fmod_studio PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/ext/fmod/api/studio/lib/libfmodstudio.dylib")
elseif(WIN32)
  set_target_properties(fmod_studio PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/ext/fmod/api/studio/lib/libfmodstudio.dylib")
endif()

set_target_properties(fmod_studio PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/ext/fmod/api/studio/inc")


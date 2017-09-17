project(Silice3D C CXX)

cmake_minimum_required(VERSION 2.8)

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
  cmake_policy(SET CMP0042 OLD)
  cmake_policy(SET CMP0048 OLD)
endif()

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION_EXTRA "")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Compiler flags
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ffast-math")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DGLAD_DEBUG")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------
add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

set (GLAD_EXTENSIONS "GL_ARB_bindless_texture")
add_subdirectory(deps/glad)

set (ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set (ASSIMP_BUILD_TESTS OFF)
add_subdirectory(deps/assimp)

set (BUILD_EXTRAS OFF)
set (BUILD_CPU_DEMOS OFF)
set (BUILD_BULLET2_DEMOS OFF)
set (BUILD_OPENGL3_DEMOS OFF)
set (BUILD_UNIT_TESTS OFF)
set (BUILD_BULLET3 OFF)
add_subdirectory(deps/bullet)

# Include dirs
set (SILICE3D_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/deps ;\
${PROJECT_SOURCE_DIR}/deps/glfw/include	;\
${PROJECT_SOURCE_DIR}/deps/glm ;\
${GLAD_INCLUDE_DIRS} ;\
${PROJECT_SOURCE_DIR}/deps/assimp/include ;\
${PROJECT_SOURCE_DIR}/deps/bullet/src ;\
${PROJECT_SOURCE_DIR}/src")

if (NOT USE_IMAGEMAGICK)
  set(SILICE3D_INCLUDE_DIRS "${SILICE3D_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}/deps/lodepng")
endif()

set(SILICE3D_INCLUDE_DIRS ${SILICE3D_INCLUDE_DIRS} PARENT_SCOPE)
include_directories(SYSTEM ${SILICE3D_INCLUDE_DIRS})

# This should be the last subdir / include
add_subdirectory(src)


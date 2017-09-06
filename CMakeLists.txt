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
include_directories(SYSTEM deps/glfw/include)

add_subdirectory(deps/glm)
include_directories(SYSTEM deps/glm)

add_subdirectory(deps/glad)
include_directories(SYSTEM ${GLAD_INCLUDE_DIRS})

set (ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set (ASSIMP_BUILD_TESTS OFF)
add_subdirectory(deps/assimp)
include_directories(SYSTEM deps/assimp/include)

set (BUILD_EXTRAS OFF)
set (BUILD_CPU_DEMOS OFF)
set (BUILD_BULLET2_DEMOS OFF)
set (BUILD_OPENGL3_DEMOS OFF)
set (BUILD_UNIT_TESTS OFF)
set (BUILD_BULLET3 OFF)
add_subdirectory(deps/bullet)
include_directories(SYSTEM deps/bullet/src)

include_directories(SYSTEM deps)

if (NOT USE_IMAGEMAGICK)
  include_directories(SYSTEM deps/lodepng)
endif()

# This should be the last subdir / include
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/src)
add_subdirectory(src)


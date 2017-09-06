cmake_minimum_required(VERSION 2.8)

if (MSVC)
    link_libraries(opengl32)
else()
    link_libraries(GL)
endif()
link_libraries(glfw)
link_libraries(glad)
link_libraries(assimp)
link_libraries(BulletDynamics)
link_libraries(BulletCollision)
link_libraries(LinearMath)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DUSE_DEBUG_CONTEXT -g")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DOGLWRAP_DEBUG=0")

if (BUILD_SHARED_LIBS)
    link_libraries("${MATH_LIBRARY}")
endif()

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (USE_IMAGEMAGICK)
  find_package(ImageMagick COMPONENTS Magick++)
  include_directories(${ImageMagick_INCLUDE_DIRS})
  link_libraries(${ImageMagick_LIBRARIES})
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_IMAGEMAGICK")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DUSE_IMAGEMAGICK")
else()
  set (LODEPNG_SOURCE "../deps/lodepng/lodepng.cpp")
endif()

file(GLOB PROJECT_SOURCE "Silice3D/*.cpp" "Silice3D/*/*.cpp" "Silice3D/*/*/*.cpp" ${LODEPNG_SOURCE})

set (PROJECT_LIBRARY_NAME "Silice3D")

add_library(${PROJECT_LIBRARY_NAME} STATIC ${PROJECT_SOURCE})

target_link_libraries(${PROJECT_LIBRARY_NAME})
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})


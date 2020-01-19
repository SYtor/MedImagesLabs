cmake_minimum_required(VERSION 3.14)
project(RGR)

set(CMAKE_CXX_STANDARD 14)

add_executable(RGR main.cpp Point.cpp Point.h Slice.cpp Slice.h Drawable.h Coordinate.h Shader.h Shader.cpp Drawable.cpp Cube.cpp Cube.h CoordinateSystem.cpp CoordinateSystem.h)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(RGR ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${FREETYPE_LIBRARIES})
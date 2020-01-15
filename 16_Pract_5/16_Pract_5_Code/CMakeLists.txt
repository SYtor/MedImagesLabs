cmake_minimum_required(VERSION 3.14)
project(Lab5)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab5 main.cpp Image.cpp Image.h Shader.cpp Shader.h TextRenderer.cpp TextRenderer.h Renderable.h DicomFileWrapper.cpp DicomFileWrapper.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(DCMTK REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${DCMTK_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(Lab5 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${DCMTK_LIBRARIES} ${FREETYPE_LIBRARIES})
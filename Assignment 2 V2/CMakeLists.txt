cmake_minimum_required(VERSION 3.17)
project(assignment02)
set(CMAKE_CXX_STANDARD 20)


link_libraries(cs4722  glfw3 opengl32 glu32)

include_directories(C:/cs4722fa22lib/include lib64/include)
link_directories(/cs4722fa22lib lib64)

add_executable(01-more-shapes 01-more-shapes/main.cpp)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/01-more-shapes)
configure_file(01-more-shapes/fragment_shader.glsl ./01-more-shapes)
configure_file(01-more-shapes/vertex_shader.glsl ./01-more-shapes)

if(EXISTS ${CMAKE_SOURCE_DIR}/02-animation/main.cpp
        AND EXISTS ${CMAKE_SOURCE_DIR}/02-animation/fragment_shader.glsl
        AND EXISTS ${CMAKE_SOURCE_DIR}/02-animation/vertex_shader.glsl)
add_executable(02-animation 02-animation/main.cpp)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/02-animation)
configure_file(02-animation/fragment_shader.glsl ./02-animation)
configure_file(02-animation/vertex_shader.glsl ./02-animation)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/03-world-coloring/main.cpp
        AND EXISTS ${CMAKE_SOURCE_DIR}/03-world-coloring/fragment_shader.glsl
        AND EXISTS ${CMAKE_SOURCE_DIR}/03-world-coloring/vertex_shader.glsl)
add_executable(03-world-coloring 03-world-coloring/main.cpp)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/03-world-coloring)
configure_file(03-world-coloring/fragment_shader.glsl ./03-world-coloring)
configure_file(03-world-coloring/vertex_shader.glsl ./03-world-coloring)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/04-object-coloring/main.cpp
        AND EXISTS ${CMAKE_SOURCE_DIR}/04-object-coloring/fragment_shader.glsl
        AND EXISTS ${CMAKE_SOURCE_DIR}/04-object-coloring/vertex_shader.glsl)
add_executable(04-object-coloring 04-object-coloring/main.cpp)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/04-object-coloring)
configure_file(04-object-coloring/fragment_shader.glsl ./04-object-coloring)
configure_file(04-object-coloring/vertex_shader.glsl ./04-object-coloring)
endif()



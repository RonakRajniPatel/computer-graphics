cmake_minimum_required(VERSION 3.17)
project(assignment07-base)



link_libraries(cs4722  glfw3 opengl32 glu32)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/cs4722fa22lib/include lib64/include)
link_directories(C:/cs4722fa22lib lib64)

add_executable(temple temple.cpp)
configure_file(vertex_shader01.glsl .)
configure_file(fragment_shader01.glsl .)
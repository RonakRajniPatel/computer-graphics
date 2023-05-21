cmake_minimum_required(VERSION 3.17)
project(assign01)

set(CMAKE_CXX_STANDARD 14)

include_directories(/cs4722fa22lib/include lib64/include)
link_directories(/cs4722fa22lib lib64)
link_libraries(cs4722 glfw3 opengl32 glu32)


add_executable(three-triangles three-triangles.cpp)
configure_file(fragment_shader.glsl .)
configure_file(vertex_shader.glsl .)


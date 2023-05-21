cmake_minimum_required(VERSION 3.17)
project(m05_advanced_textures)



link_libraries(cs4722 glfw3 opengl32 glu32)

set(CMAKE_CXX_STANDARD 20)

include_directories(c:/cs4722fa22lib/include lib64/include)
link_directories(c:/cs4722fa22lib lib64)

add_executable(01-sky-box 01-sky-box/skybox.cpp)
configure_file(01-sky-box/vertex_shader01.glsl .)
configure_file(01-sky-box/fragment_shader01.glsl .)



#add_executable(03A-image-processing 03A-image-processing/image-processing.cpp
#        03A-image-processing/setup_scene.cpp 03A-image-processing/setup_view_in_view.cpp)
#configure_file(03A-image-processing/image_processing_vertex_shader.glsl .)
#configure_file(03A-image-processing/image_processing_fragment_shader.glsl .)
#configure_file(03A-image-processing/scene_fragment_shader.glsl .)
#configure_file(03A-image-processing/scene_vertex_shader.glsl .)

#add_executable(03A2-IP 03A2-IP/image-processing.cpp 03A2-IP/scene2.cpp)


cmake_minimum_required(VERSION 3.17)
project(m05_advanced_textures)



link_libraries(cs4722 glfw3 opengl32 glu32)

set(CMAKE_CXX_STANDARD 20)

include_directories(c:/cs4722fa22lib/include lib64/include)
link_directories(c:/cs4722fa22lib lib64)

add_executable(01-sky-box 01-sky-box/skybox.cpp)
configure_file(01-sky-box/vertex_shader01.glsl .)
configure_file(01-sky-box/fragment_shader01.glsl .)

add_executable(02-reflecting-object 02-reflecting-object/reflecting-object.cpp)
configure_file(02-reflecting-object/fragment_shader02.glsl .)
configure_file(02-reflecting-object/vertex_shader02.glsl .)

#add_executable(03A-image-processing 03A-image-processing/image-processing.cpp
#        03A-image-processing/setup_scene.cpp 03A-image-processing/setup_view_in_view.cpp)
#configure_file(03A-image-processing/image_processing_vertex_shader.glsl .)
#configure_file(03A-image-processing/image_processing_fragment_shader.glsl .)
#configure_file(03A-image-processing/scene_fragment_shader.glsl .)
#configure_file(03A-image-processing/scene_vertex_shader.glsl .)

#add_executable(03A2-IP 03A2-IP/image-processing.cpp 03A2-IP/scene2.cpp)

add_executable(03A-image-processing-static-image 03A-image-processing-static-image/file-image.cpp)
configure_file(03A-image-processing-static-image/fragment_shader_03A.glsl .)
configure_file(03A-image-processing-static-image/vertex_shader_03A.glsl .)

add_executable(03B-image-processing-rendered-image
        03B-image-processing-rendered-image/file-image.cpp
        03B-image-processing-rendered-image/reflecting-object.cpp)

add_executable(03C-pixel-filters 03C-pixel-filters/pixel_filters.cpp)
configure_file(03C-pixel-filters/vertex_shader03C.glsl .)
configure_file(03C-pixel-filters/fragment_shader03C.glsl .)


add_executable(04-dynamic-reflection
        04-dynamic-reflection/dynamic-reflection.cpp
        04-dynamic-reflection/scene1.cpp
        04-dynamic-reflection/scene2.cpp)
configure_file(04-dynamic-reflection/fragment_shader04-1.glsl .)
configure_file(04-dynamic-reflection/vertex_shader04-1.glsl .)
configure_file(04-dynamic-reflection/fragment_shader04-2.glsl .)
configure_file(04-dynamic-reflection/vertex_shader04-2.glsl .)


add_executable(05-point-sprites 05-point-sprites/point_sprites.cpp)
configure_file(05-point-sprites/vertex_shader05.glsl .)
configure_file(05-point-sprites/fragment_shader05.glsl .)

add_executable(06-point-sprites-2 06-point-sprites-2/point_sprites-2.cpp)
configure_file(06-point-sprites-2/vertex_shader06.glsl .)
configure_file(06-point-sprites-2/fragment_shader06.glsl .)

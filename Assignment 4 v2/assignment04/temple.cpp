
/*
 * array of columns
 * pan camera around z axis
 *
 */

#include "GLM/gtc/type_ptr.hpp"
#include <glad/gl.h>

#include "learnopengl/shader.h"

#include "cs4722/artifact.h"
#include "cs4722/view.h"
#include "cs4722/texture_utilities.h"
#include "callbacks.h"
//#include "../artifact_rt2.h"


static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact*> artifact_list;

const static auto b_position = 1;
const static auto b_texture_coords = 2;
const static auto b_color = 3;

static GLint u_transform = 1;
static GLint u_sampler = 2;
static GLint u_surface_effect = 3;
static GLint u_diffuse_color = 4;

#include "STB/stb_image.h"




void init()
{

    the_view = new cs4722::view();
    the_view->enable_logging = false;
//    the_view->set_flup(glm::vec3(0.202971,-0.227978,-0.952277),
//                       glm::vec3(-0.978031,0,-0.20846),
//                       glm::vec3(0.0475242,0.973667,-0.222969),
//                       glm::vec3(-0.146291,0.195185,1.3214));




    Shader shader("vertex_shader01.glsl","fragment_shader01.glsl");
    program = shader.ID;
    glUseProgram(program);

    glEnable(GL_DEPTH_TEST);

    //change 1
    cs4722::init_texture_from_file("../media/grass_texture_02.jpg", 1);
    auto wrap_type = GL_MIRRORED_REPEAT;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_type);


    cs4722::init_texture_from_file("../media/colorado-river-canyonlands-national-park.jpg", 2);
    cs4722::init_texture_from_file("../media/pexels-photo-814499.jpeg", 3);
    cs4722::init_texture_from_file("../media/tulips-bed-256x256.png", 4);
    cs4722::init_texture_from_file("../media/8.jpg", 0);
    //end



    auto* block = new cs4722::block();
    block->color_set_ = std::vector<cs4722::color>({cs4722::x11::forest_green, cs4722::x11::sienna});
    auto* cylinder = new cs4722::cylinder();
    cylinder->color_set_ = std::vector<cs4722::color>({cs4722::x11::grey70, cs4722::x11::grey100});


    auto number = 25;
    auto radius = .25;
    auto height = 5.0 * radius;
    auto gap = 4.0 * radius;
    auto inter_center = 2.0 * radius + gap;
    auto edge_gap = 0.5 * gap;
    auto base = glm::vec4(edge_gap, edge_gap, 0, 1);
    auto ground_width = inter_center * (number - 1) + 2 * edge_gap + 2 * radius;
    auto ground_depth = ground_width;



    the_view->camera_position = glm::vec3(0, 0, radius);
    the_view->camera_up = glm::vec3(0,0,1);
    the_view->camera_forward = glm::normalize(glm::vec3(1, 1, 0));
    the_view->camera_left = glm::cross(the_view->camera_up, the_view->camera_forward);


    for(int x = 0; x < number; x++) {
        for(int y = 0; y < number; y++) {
            auto artf = new cs4722::artifact_rotating();
            artf->the_shape = cylinder;
            artf->world_transform.scale = glm::vec3(radius, height, radius);
            auto where = base + glm::vec4(x * inter_center, y*inter_center, 0, 0);
            artf->world_transform.translate = where + glm::vec4(0,0,height / 2,0);
            artf->world_transform.rotation_angle = M_PI / 2;
            artf->world_transform.rotation_axis = glm::vec3(1,0,0);
            artf->animation_transform.rotation_center = glm::vec3(number, number, 0);
            artf->animation_transform.rotation_center =
                    artf->world_transform.matrix() * glm::vec4(1,0,0,1);
            auto ca = 200;
            auto cb = 255 - ca;

            //change 2
            unsigned int r = (int) x * ca / (number-1) + cb;
            unsigned int g = (int) y * ca / (number-1) + cb;
            unsigned int b = (int) (x + y) * ca /(2*number-2) + cb;

            artf->surface_material.diffuse_color = cs4722::color(r,g,b);
            artf->surface_effect = (2*x + 3*y + 4) % 5;
            artf->texture_unit = (y % 3) + 2;
            //end

            artifact_list.push_back(artf);



            artf = new cs4722::artifact_rotating();
            artf->the_shape = block;
            artf->world_transform.scale = glm::vec3(radius, inter_center , radius);
            artf->world_transform.translate = where + glm::vec4(0, 0, height + radius/2 , 0);
            artf->surface_material.diffuse_color = cs4722::x11::cornflower_blue;
            artf->animation_transform.rotation_axis = glm::vec3(0,0,1);
            artf->animation_transform.rotation_center = where;
            if(x % 2 == 0) {
                if (y % 2 == 0) {
                    artf->rotation_offset = M_PI;
                } else {
                    artf->rotation_offset = M_PI / 2;
                }
            } else {
                if (y % 2 == 0) {
                    artf->rotation_offset = 3* M_PI / 2;
                } else {
                    artf->rotation_offset = 0;
                }
            }

            artf->rotation_rate = M_PI/3;
            artf->surface_material.diffuse_color = cs4722::x11::brown;

            //change 3
            artf->surface_effect = b_texture_coords;
            artf->texture_unit = 0;
            //end
            artifact_list.push_back(artf);

        }
    }

    //change 4
    auto artf = new cs4722::artifact_rotating;
    auto bl = new cs4722::block();
    bl->texture_scale = number;
    artf->the_shape = bl;
    artf->world_transform.scale = glm::vec3(ground_width, ground_depth, .01);
    artf->world_transform.translate = glm::vec3(ground_width / 2, ground_depth / 2, 0);
    artf->surface_material.diffuse_color = cs4722::x11::lawn_green;
    artf->surface_effect = b_texture_coords;
    artf->texture_unit = 1;
    //end
    artifact_list.push_back(artf);



    auto total_vertex_count = 0;
    for(auto *artf : artifact_list) {
        if(artf->the_shape->buffer_size == 0) {
            artf->the_shape->buffer_start = total_vertex_count;
            artf->the_shape->buffer_size = artf->the_shape->get_size();
            total_vertex_count += artf->the_shape->get_size();
        } else {
            artf->shape_is_shared = true;
        }
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    const int number_of_buffers = 3;
    GLuint  buffers[number_of_buffers];
    glGenBuffers(number_of_buffers, buffers);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * total_vertex_count, NULL,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_position);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * total_vertex_count, NULL,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_texture_coords, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_texture_coords);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, 1 * 4 * total_vertex_count, NULL,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(b_color);


    for (auto artf : artifact_list) {
        if(!artf->shape_is_shared) {
            auto pos = artf->the_shape->positions();
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferSubData(GL_ARRAY_BUFFER,
                            4 * 4 * artf->the_shape->buffer_start,
                            4 * 4 * artf->the_shape->buffer_size,
                            pos->data());
            delete pos;

            auto texture_coords = artf->the_shape->texture_coordinates();
            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferSubData(GL_ARRAY_BUFFER,
                            2 * 4 * artf->the_shape->buffer_start,
                            2 * 4 * artf->the_shape->buffer_size,
                            texture_coords->data());
            delete texture_coords;

            auto colors = artf->the_shape->colors();
            glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
            glBufferSubData(GL_ARRAY_BUFFER,
                            1 * 4 * artf->the_shape->buffer_start,
                            1 * 4 * artf->the_shape->buffer_size,
                            colors->data());
            delete colors;
        }
    }

}


void render()
{

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto artf : artifact_list) {

        artf->animate(time, delta_time);

        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(u_transform, 1, GL_FALSE, glm::value_ptr(transform));
        /*
         * Set the texture unit to use for this artf
         *
         * The next two lines are commented out to stop lots of warning messages
         * when the code is not actually using these two uniform variables.
         * Uncomment these two lines when you get the fragment shader code expanded.
         */

        //changes 6
        glUniform1i(u_sampler, artf->texture_unit);
        glUniform1i(u_surface_effect, artf->surface_effect);
        //end

        glUniform4fv(u_diffuse_color, 1, artf->surface_material.diffuse_color.as_float().get());
        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


int
main(int argc, char** argv)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    auto aspect_ratio = 18.0f/9.0f;
    float ratio = 0.9f;             // ratio of window size to screen size
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
    //std::cout << xpos << " " << ypos << " " << width << " " << height << std::endl;
    int w_width = glm::min(ratio * width, aspect_ratio * ratio * height);
    int w_height = w_width / aspect_ratio;
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Multiple Textures", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);
//    cs4722::setup_debug_callbacks();

    init();
    the_view->perspective_aspect = aspect_ratio;

    glfwSetWindowUserPointer(window, the_view);
    glfwSetKeyCallback(window, general_key_callback);
    glfwSetCursorPosCallback(window, move_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetWindowUserPointer(window, the_view);
	
    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);

        render();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwDestroyWindow(window);

    glfwTerminate();
}

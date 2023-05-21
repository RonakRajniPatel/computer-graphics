//
// Created by Ben on 9/28/2022.
//


#include "GLM/gtc/type_ptr.hpp"
#include <glad/gl.h>

#include "learnopengl/shader.h"

#include "cs4722/artifact.h"
#include "cs4722/view.h"
#include "cs4722/texture_utilities.h"
//#include "../callbacks.h"

#include "scene1.h"

//cs4722::view *scene1_view;
static GLuint program;
static GLuint vao;
static std::vector<cs4722::artifact*> artifact_list;

const static auto b_position = 1;
const static auto b_texture_coords = 2;

static GLint u_transform = 1;
static GLint u_sampler = 2;
static GLint u_surface_effect = 3;
static GLint u_diffuse_color = 4;
static GLint u_smapler_c = 5;

void scene1_init()
{


    Shader shader("vertex_shader04-1.glsl","fragment_shader04-1.glsl");
    program = shader.ID;
    glUseProgram(program);

    glEnable(GL_DEPTH_TEST);

    cs4722::artifact_rotating *artf;

    cs4722::shape* b = new cs4722::sphere(15, 50);
    artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(-.5, -.5, 0));
    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    artf->rotation_rate = (M_PI / 3);
    artf->world_transform.scale = (glm::vec3(.4f, .4f, .4f));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
    /*
     * Each artf can be assigned a texture unit, this is used in the display function
     */
    artf->texture_unit = 1;
    artf->surface_material.diffuse_color = cs4722::x11::violet_red;
    artf->surface_effect = 1;
    artifact_list.push_back(artf);


    cs4722::shape* b1 = new cs4722::block();
    auto* artf1 = new cs4722::artifact_rotating();
    artf1->the_shape = (b1);
    artf1->world_transform.translate = (glm::vec3(-.5, .5, 0));
    artf1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    artf1->rotation_rate = (M_PI / 3);
    artf1->world_transform.scale = (glm::vec3(.4f, .4f, .4f));
    artf1->animation_transform.rotation_center =
            artf1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    artf1->texture_unit = 2;
    artf1->surface_material.diffuse_color = cs4722::x11::slate_blue;
    artf1->surface_effect = 0;
    artifact_list.push_back(artf1);

    b1 = new cs4722::cylinder();
    artf1 = new cs4722::artifact_rotating();
    artf1->the_shape = (b1);
    artf1->world_transform.translate = (glm::vec3(.5, .5, 0));
    artf1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    artf1->rotation_rate = (M_PI / 3);
    artf1->world_transform.scale = (glm::vec3(.4f, .4f, .4f));
    artf1->animation_transform.rotation_center =
            artf1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    artf1->texture_unit = 1;
    artf1->surface_material.diffuse_color = cs4722::x11::olive_drab;
    artf1->surface_effect = 2;
    artifact_list.push_back(artf1);


    auto skybox_scale = 40000.0f;
    auto* a_shape = new cs4722::block();
    artf = new cs4722::artifact_rotating();
    artf->the_shape = a_shape;
    artf->world_transform.scale = glm::vec3(skybox_scale);
    artf->texture_unit = 32;
    artf->surface_effect = 32;
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

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    const int number_of_buffers = 2;
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
        }
    }

}


void scene1_render(cs4722::view* the_view)
{
    glBindVertexArray(vao);
    glUseProgram(program);

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

        if(artf->texture_unit >= 32)
            glUniform1i(u_smapler_c, artf->texture_unit);
        else
            glUniform1i(u_sampler, artf->texture_unit);

        glUniform1i(u_surface_effect, artf->surface_effect);
        glUniform4fv(u_diffuse_color, 1, artf->surface_material.diffuse_color.as_float().get());

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


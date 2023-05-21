//
// Created by Ben on 9/28/2022.
//


#include "GLM/gtc/type_ptr.hpp"
#include <GLM/gtc/matrix_inverse.hpp>
#include <glad/gl.h>

#include "learnopengl/shader.h"

#include "cs4722/artifact.h"
#include "cs4722/view.h"
#include "cs4722/texture_utilities.h"

#include "scene2.h"


static GLuint program;
static GLuint vao;
static std::vector<cs4722::artifact*> artifact_list;

const static auto b_position = 1;
const static auto b_texture_coords = 2;
const static auto b_normal = 3;


static GLint u_vp_transform = 1;
static GLint u_n_transform = 2;
static GLint u_m_transform = 3;
static GLint u_sampler2 = 4;
static GLint u_samplerc = 5;
static GLint u_surface_effect = 6;
static GLint u_camera_position = 7;
static GLint u_diffuse_color = 8;


void scene2_init()
{

    Shader shader("vertex_shader04-2.glsl","fragment_shader04-2.glsl");
    program = shader.ID;
    glUseProgram(program);

    glEnable(GL_DEPTH_TEST);

    auto rtf = artifact_list;

    cs4722::artifact_rotating *artf = new cs4722::artifact_rotating();
    artf->the_shape = new cs4722::block();
//    artf->the_shape = new cs4722::sphere();
//    artf->world_transform.scale = glm::vec3(.5, .5, .5);
//    artf->world_transform.scale = glm::vec3(2,2,2);
//    artf->world_transform.scale = glm::vec3(4,4,4);
    artf->world_transform.scale = glm::vec3(8,8,8);
    artf->world_transform.translate = block_position;
    artf->surface_material.diffuse_color = cs4722::x11::sienna;
    artf->surface_effect = 33;
//    artf->texture_unit = 32;
    artf->texture_unit = fb_texture_unit;
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
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * total_vertex_count, NULL,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_normal, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_normal);

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

            auto nrm = artf->the_shape->normals();
            glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
            glBufferSubData(GL_ARRAY_BUFFER,
                            4 * 4 * artf->the_shape->buffer_start,
                            4 * 4 * artf->the_shape->buffer_size,
                            nrm->data());
            delete nrm;
        }
    }

}


void scene2_render(cs4722::view *the_view)
{
//    auto rtf = artifact_list;

    glBindVertexArray(vao);
    glUseProgram(program);

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;
    glUniformMatrix4fv(u_vp_transform, 1, GL_FALSE, glm::value_ptr(vp_transform));
    glUniform3fv(u_camera_position, 1, glm::value_ptr(the_view->camera_position));


    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto artf : artifact_list) {

        artf->animate(time, delta_time);

        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(u_m_transform, 1, GL_FALSE, glm::value_ptr(model_transform));
        glm::mat4 n_transform = glm::inverseTranspose(model_transform);
        // send model and normal transformations to the vertex shader
        glUniformMatrix4fv(u_m_transform, 1, GL_FALSE, glm::value_ptr(model_transform));
        glUniformMatrix4fv(u_n_transform, 1, GL_FALSE, glm::value_ptr(n_transform));

        /*
         * Set the texture unit to use for this artf
         */
//        if(artf->surface_effect == 33) {
            glUniform1i(u_samplerc, artf->texture_unit);
//        } else {
            glUniform1i(u_sampler2, artf->texture_unit);
//        }

        glUniform1i(u_surface_effect, artf->surface_effect);
        glUniform4fv(u_diffuse_color, 1, artf->surface_material.diffuse_color.as_float().get());

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


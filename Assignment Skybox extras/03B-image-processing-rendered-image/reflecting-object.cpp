

/*
 * Reflecting objects use the background cube texture as their texture source.
 * However, the sample is chosen by reflecting the camera to fragment vector in the surface.
 * In this way the surface acts as a mirror.
 *
 * This example does not reflect the scene objects.  How to do this is discussed in the notes.
 * We haven't implemented that in OpenGL, but there is a WebGL example you can examine.  See the notes.
 */


#include <GLM/gtc/matrix_inverse.hpp>
 #include <GLM/gtc/type_ptr.hpp>



#include <glad/gl.h>

#include "learnopengl/shader.h"

#include "cs4722/view.h"
#include "cs4722/artifact.h"
#include "cs4722/texture_utilities.h"

#include "reflecting_object.h"


static cs4722::view *the_view;
static GLint program;
static GLuint vao;
static std::vector<cs4722::artifact*> artifact_list;

const static GLuint b_position = 1;
const static GLuint b_texture_coord = 2;
const static GLuint b_normal = 3;

const static GLuint u_vp_transform = 1;
const static GLuint u_n_transform = 2;
const static GLuint u_m_transform = 3;
const static GLuint u_sampler2 = 4;
const static GLuint u_samplerC = 5;
const static GLuint u_surface_effect = 6;
const static GLuint u_camera_position = 7;




cs4722::view* reflecting_init()
{

    the_view = new cs4722::view();
    the_view->enable_logging = 0;
    the_view->set_flup(glm::vec3(0.202971,-0.227978,-0.952277),
                       glm::vec3(-0.978031,0,-0.20846),
                       glm::vec3(0.0475242,0.973667,-0.222969),
                       glm::vec3(-0.146291,0.195185,1.3214));



//    printf("ready to compile\n");
    Shader shader("vertex_shader02.glsl","fragment_shader02.glsl");
    program = shader.ID;
    if(program <= 0) {
        printf("program is %d\n", program);
        exit(23);
    }
    glUseProgram(program);

//    glEnable(GL_DEPTH_TEST);


    cs4722::init_texture_from_file("../media/tulips-bed-2048x2048.png", 2);
    cs4722::init_texture_computed(1, 8);
    cs4722::init_cube_texture_from_path("../media/fjaderholmarna", 10, "png");
//    cs4722::init_cube_texture_from_path("../media/oriented-cube", 10, "png");

    cs4722::shape* b = new cs4722::sphere(15, 50);
    auto* artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(.5, -.5, 0));
    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    artf->rotation_rate = (M_PI / 3);
    artf->world_transform.scale = (glm::vec3(.4f, .6f, .4f));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
    artf->texture_unit = 1;
    artf->surface_effect = 0;
    artifact_list.push_back(artf);


    b = new cs4722::block();
    artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(-.5, 0, 0));
    artf->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    artf->rotation_rate = (M_PI / 3);
    artf->world_transform.scale = (glm::vec3(.2f, .6f, .2f));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    artf->texture_unit = 2;
    artf->surface_effect = 0;

    artifact_list.push_back(artf);

    auto skybox_scale = 40000.0f;

    auto* a_shape = new cs4722::block();
    // auto* a_shape = new cs4722::sphere();
    // auto* a_shape = new cs4722::cylinder();
    // auto* a_shape = new cs4722::torus();

    auto artfb = new cs4722::artifact();
    artfb->the_shape = a_shape;
    artfb->world_transform.scale = glm::vec3(skybox_scale);
    artfb->texture_unit = 10;
    artfb->surface_effect = 1;

    artifact_list.push_back(artfb);


    /*
     * Reflecting objects
     * These are noted with surface_effect 2 for the fragment shader
     * They use the same cube texture unit as the skybox, just select the sample with
     *      different parameters.
     */

    b = new cs4722::sphere(15, 50);
    auto* obj = new cs4722::artifact_rotating();
    obj->the_shape = (b);
    obj->world_transform.translate = (glm::vec3(.5, 5, 0));
    obj->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
//    obj->rotation_rate = 2*M_PI / 10;
    obj->world_transform.scale = (glm::vec3(1.0f, .6f, 1.0f));
    obj->animation_transform.rotation_center =
            obj->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
    obj->surface_effect = 2;
    obj->texture_unit = 10;
    artifact_list.push_back(obj);


    cs4722::shape* b1 = new cs4722::block();
//   b1 = new cs4722::torus();
//    b1 = new cs4722::sphere();
   auto* obj1 = new cs4722::artifact_rotating();
    obj1->the_shape = (b1);
    obj1->world_transform.translate = (glm::vec3(-.5, 0, 3));
    obj1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    obj1->rotation_rate = 2 * M_PI / 15;
    obj1->world_transform.scale = (glm::vec3(1.0f, .6f, 1.0f));
    obj1->animation_transform.rotation_center =
            obj1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    obj1->surface_effect = 2;
    obj1->texture_unit = 10;
    artifact_list.push_back(obj1);





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


    /*
     * We need the normal vectors to compute reflection.
     * Normal vectors will be discussed in detail in the next module, lighting and shading.
     */


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    const int number_of_buffers =  3;
    GLuint  buffers[number_of_buffers];
    glGenBuffers(number_of_buffers, buffers);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * total_vertex_count, NULL,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_position);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * total_vertex_count, NULL,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_texture_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_texture_coord);

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
    return the_view;
}


void reflecting_render()
{
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
    glBindVertexArray(vao);
    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;
    // while the transform of a vertex in the vertex shader will be equialent to what
    // we have done before, a different set of transformations is sent to the vertex
    // shader to support computing the vertex position in other coordinate systems needed
    // to compute reflections.
    glUniformMatrix4fv(u_vp_transform, 1, GL_FALSE, glm::value_ptr(vp_transform));
    /*
     * The camera position is needed to compute the reflection
     */
    glUniform3fv(u_camera_position, 1, glm::value_ptr(the_view->camera_position));

    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto artf : artifact_list) {
        artf->animate(time, delta_time);
        glm::mat4 model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        /*
         * Normal vectors need to be transformed to world coordinates to compute the reflection.
         * The matrix that does that transformation is the inverse of the transpose of the model transform.
         */
        glm::mat4 n_transform = glm::inverseTranspose(model_transform);
        // send model and normal transformations to the vertex shader
        glUniformMatrix4fv(u_m_transform, 1, GL_FALSE, glm::value_ptr(model_transform));
        glUniformMatrix4fv(u_n_transform, 1, GL_FALSE, glm::value_ptr(n_transform));

        // it is probably safer to check which way the texture unit value is to be used
        //      regular texture or cube texture.
        if(artf->surface_effect == 0) {
            glUniform1i(u_sampler2, artf->texture_unit);
        } else {
            glUniform1i(u_samplerC, artf->texture_unit);
        }

        glUniform1i(u_surface_effect, artf->surface_effect);

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


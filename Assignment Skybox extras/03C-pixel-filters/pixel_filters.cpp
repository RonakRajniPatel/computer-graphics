
/*
 * High-end cameras can be fitted with filters, usually a piece of flat glass, whose characteristics will modify the
 * image recorded.
 * For example, if the filter is red, then the image recorded will only hold the red component of the light
 * from the scene.
 *
 * A similar effect can be implemented in OpenGL by applying various functions to a fragment.
 *
 * All the interesting coding is in the fragment shader.  You should try out the various options
 * available in the fragment shader to see what effect they have.
 *
 * The next example, image processing, is more complex since that involves changing fragments based on the color
 * of multiple fragments.
 */


 #include <GLM/gtc/type_ptr.hpp>



#include <glad/gl.h>

#include "learnopengl/shader.h"

#include "cs4722/view.h"
#include "cs4722/artifact.h"
#include "cs4722/texture_utilities.h"

#include "../callbacks.h"


static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact*> artifact_list;

const static GLuint b_position = 1;
const static GLuint b_texture_coord = 2;


const static GLuint u_transform = 1;
const static GLuint u_sampler2 = 2;
const static GLuint u_samplerC = 3;
const static GLuint u_surface_effect = 4;




static GLuint vao;


void init()
{

    the_view = new cs4722::view();
    the_view->enable_logging = 0;
    the_view->set_flup(glm::vec3(0.202971,-0.227978,-0.952277),
                       glm::vec3(-0.978031,0,-0.20846),
                       glm::vec3(0.0475242,0.973667,-0.222969),
                       glm::vec3(-0.146291,0.195185,1.3214));



    printf("ready to compile\n");
    Shader shader("vertex_shader03C.glsl","fragment_shader03C.glsl");
    program = shader.ID;
    printf("program is %d\n", program);
    glUseProgram(program);


    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);


    cs4722::init_texture_from_file("../media/tulips-bed-2048x2048.png", 2);
    cs4722::init_texture_computed(1, 8);
    cs4722::init_cube_texture_from_path("../media/fjaderholmarna", 10, "png");
//    cs4722::init_cube_texture_from_path("../media/oriented-cube-reversed", 10, "png");

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

    artf = new cs4722::artifact_rotating();
    artf->the_shape = a_shape;
    artf->world_transform.scale = glm::vec3(skybox_scale);
    artf->texture_unit = 10;
    artf->surface_effect = 1;

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
    glVertexAttribPointer(b_texture_coord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_texture_coord);

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


//----------------------------------------------------------------------------
//
// display
//


void display()
{

    // bind the VAO for this set of vertex buffers
    glBindVertexArray(vao);
    // since we have multiple sets of shader programs, we must activate the one we want to use
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

        glUniform1i(u_sampler2, artf->texture_unit);
        glUniform1i(u_samplerC, artf->texture_unit);

        glUniform1i(u_surface_effect, artf->surface_effect);

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


int
main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto aspect_ratio = 16.0f/9.0f;
    float ratio = 0.9f;             // ratio of window size to screen size
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
    //std::cout << xpos << " " << ypos << " " << width << " " << height << std::endl;
    int w_width = glm::min(ratio * width, aspect_ratio * ratio * height);
    int w_height = w_width / aspect_ratio;
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Pixel Filters", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);;



    gladLoadGL(glfwGetProcAddress);
//    cs4722::setup_debug_callbacks();

    init();

    glfwSetWindowUserPointer(window, the_view);
//    cs4722::setup_user_callbacks(window);

//    the_view->enable_logging = false;

    printf("view logging %d\n", the_view->enable_logging);

    glfwSetKeyCallback(window,general_key_callback);
    glfwSetCursorPosCallback(window,move_callback);
    glfwSetWindowSizeCallback(window,window_size_callback);
	
    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
//        printf("view logging %d\n", the_view->enable_logging);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}




#include "GLM/gtc/type_ptr.hpp"
#include <GLM/gtc/matrix_inverse.hpp>
#include <glad/gl.h>

#include "learnopengl/shader.h"

#include "cs4722/artifact.h"
#include "cs4722/view.h"
#include "cs4722/texture_utilities.h"
#include "../callbacks.h"


#include "scene1.h"
#include "scene2.h"

//static auto cube_image_path = "../media/blue_cloud"; // jpg
static auto cube_image_path = "../media/oriented-cube-reversed";
//static auto cube_image_path = "../media/fjaderholmarna";

void setup_textures() {
    /*
    *  Create two textures, one from a file and one procedurally generated
       Note the different texture units assigned.
    */
    cs4722::init_texture_from_file("../media/tulips-bed-2048x2048.png", 2);
    cs4722::init_texture_computed(1, 128);

//        cs4722::init_cube_texture_from_path(cube_image_path, 32, "png");
//        cs4722::init_cube_texture_from_path("../media/fjaderholmarna", 32, "png");
//    cs4722::init_cube_texture_from_path("../media/oriented-cube-reversed", 32, "png");
    cs4722::init_cube_texture_from_path("../media/blue_cloud", 32, "jpg");


}


//const auto fb_texture_unit = 61;
const auto frame_buffer_width = 2048;
const auto frame_buffer_height = frame_buffer_width;

/*
 * Create a framebuffer that renders an image to a texture.
 * The framebuffer includes a depth buffer so that
 */
static GLuint setup_frame_buffer()
{
    // create a texture to receive the rendered image
    glActiveTexture(GL_TEXTURE0 + fb_texture_unit);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, frame_buffer_width, frame_buffer_height);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//    GLuint texture = cs4722::init_cube_texture_from_path(cube_image_path, 32, "png");

    // create a framebuffer
    GLuint fb;
    glGenFramebuffers(1, &fb);
    // attach the texture as the place to receive the rendered image
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_Z ,texture, 0);

    // attach a depth buffer to the framebuffer
    GLuint depth_buffer;
    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frame_buffer_width, frame_buffer_height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depth_buffer);

    // Got quite a bit of help at this site setting this up:
    //  http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

    // checking that the framebuffer was set up properly
    std::cout << "frame buffer complete? " <<
              glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) <<
              "(should be " << GL_FRAMEBUFFER_COMPLETE << ")" <<
              std::endl;

    return fb;
}

void scene_setup_for_fb(GLuint frame_buffer) {
//    the_view->reverse_camera();
//    the_view->camera_forward = -the_view->camera_forward;
//    the_view->camera_left = -the_view->camera_left;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
    glViewport(0, 0, frame_buffer_width, frame_buffer_height);
//    parts_display();
//    the_view->camera_forward = -the_view->camera_forward;
//    the_view->camera_left = -the_view->camera_left;
}

/*
 * Bind to the window as the target of the next rendering.
 */
void scene_setup_for_window(GLFWwindow* window) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    int w_width, w_height;
    glfwGetFramebufferSize(window, &w_width, &w_height);
    glViewport(0, 0, w_width, w_height);

}

int
main(int argc, char** argv)
{
    auto* view1 = new cs4722::view();
    auto* view2 = new cs4722::view();

    view1->enable_logging = false;
    view2->enable_logging = false;
//    view1->set_flup(glm::vec3(0.202971,-0.227978,-0.952277),
//                          glm::vec3(-0.978031,0,-0.20846),
//                          glm::vec3(0.0475242,0.973667,-0.222969),
//                          glm::vec3(-0.146291,0.195185,1.3214));
    view1->camera_position = glm::vec3(0,0,2);
    view1->camera_forward = glm::vec3(0,0,-1);
    view1->camera_up = glm::vec3(0,1,0);
    view1->camera_left = glm::cross(view1->camera_up, view1->camera_forward);



    view2->camera_position = block_position;
    view2->camera_forward = glm::vec3(0,0,1);
    view2->camera_up = glm::vec3(0,-1,0);
    view2->camera_left = glm::cross(view2->camera_up, view2->camera_forward);




    glfwInit();

    // set the OpenGL version to use: 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    auto aspect_ratio = 16.0f/9.0f;
    float ratio = 0.9f;             // ratio of window size to screen size
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
    //std::cout << xpos << " " << ypos << " " << width << " " << height << std::endl;
    int w_width = glm::min(ratio * width, aspect_ratio * ratio * height);
    int w_height = w_width / aspect_ratio;
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Render to Texture Prelim 2", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);


    gladLoadGL(glfwGetProcAddress);
    setup_textures();
//    cs4722::setup_debug_callbacks();

//    glViewport(0, w_height/2, w_width/2, w_height/2);



    scene1_init();
    scene2_init();
    view1 ->perspective_aspect = aspect_ratio;
    view2->perspective_aspect = aspect_ratio;

    auto frame_buffer = setup_frame_buffer();
//    auto frame_buffer = 0;

//    scene1_view->perspective_aspect = aspect_ratio;

    glfwSetKeyCallback(window, general_key_callback);
    glfwSetCursorPosCallback(window, move_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetWindowUserPointer(window, view1);

    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);

        scene_setup_for_fb(frame_buffer);
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::navajo_white.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, frame_buffer_width, frame_buffer_height);
        scene1_render(view2);
        scene_setup_for_window(window);
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, w_width, w_height);
        scene1_render(view1);
        scene2_render(view1);
//        glViewport(w_width/2, 0, w_width/2, w_height/2);
//        scene1_render(view2);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwDestroyWindow(window);

    glfwTerminate();
}

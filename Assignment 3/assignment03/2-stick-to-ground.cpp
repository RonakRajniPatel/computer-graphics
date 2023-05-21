


#include "GLM/gtc/type_ptr.hpp"
#include "GLM/gtc/random.hpp"
#include <glad/gl.h>

#include "learnopengl/shader.h"
#include "cs4722/artifact.h"
#include "cs4722/view.h"

static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact *> artf_list;

// locations of variables in the shaders
const static auto u_transform = 1;
const static auto uColor=2;
const static auto b_position = 1;
const static auto b_color = 2;


static auto asteroid_center = glm::vec3(0, 0, 0);
static float asteroid_radius = 20.0;
static float pc_height = 0.1;

static unsigned int random_seed = 2718281828;

void init() {

    the_view = new cs4722::view();
//    the_view->camera_position = glm::vec3(0, 0, 3);

    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");
    program = shader.ID;
    glUseProgram(program);

    glEnable(GL_DEPTH_TEST);


    cs4722::shape *b = new cs4722::sphere(100, 200);
    b->color_set_ = std::vector<cs4722::color>({cs4722::color(210, 255, 210), cs4722::color(230, 255, 230)});
    auto *artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = asteroid_center;
    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    artf->world_transform.scale = (glm::vec3(asteroid_radius, asteroid_radius, asteroid_radius));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
    artf->surface_material.diffuse_color = cs4722::x11::white;
    artf_list.push_back(artf);


    std::srand(random_seed);


    cs4722::shape *shapes[] = {
            new cs4722::block(),
            new cs4722::cylinder(.3, 20),
            new cs4722::cylinder(1.2, 20),
    };
    int num_shapes = 3;
    shapes[0]->color_set_ = std::vector<cs4722::color>({cs4722::x11::gray60, cs4722::x11::gray100});
    shapes[1]->color_set_ = std::vector<cs4722::color>({cs4722::x11::gray60, cs4722::x11::gray100});
    shapes[2]->color_set_ = std::vector<cs4722::color>({cs4722::x11::gray60, cs4722::x11::gray100});

    for (int i = 0; i < 5000; i++) {
        float height = glm::linearRand(.01, .2);
        auto position = glm::sphericalRand(asteroid_radius + height / 2) + asteroid_center;
//        printf("pos (%f, %f, %f)  distance %f\n", position.x, position.y, position.z,
//               glm::length(position - asteroid_center));
        auto cToP = position - asteroid_center;

        artf = new cs4722::artifact_rotating();
        artf->the_shape = shapes[i % num_shapes];
        artf->world_transform.scale = glm::vec3(height / 4, height, height / 4);
        artf->world_transform.rotation_axis = glm::vec3(cToP.z, 0, -cToP.x);
        artf->world_transform.rotation_angle = glm::acos(position.y / glm::length(position));
        artf->world_transform.rotation_center = glm::vec3(0, 0, 0);
        artf->world_transform.translate = position;

        unsigned int r = (int) glm::linearRand(0, 256);
        unsigned int g = (int) glm::linearRand(0, 256);
        unsigned int b = (int) glm::linearRand(0, 256);
        artf->surface_material.diffuse_color = cs4722::color(r, g, b);

        artf_list.push_back(artf);
    }



    // position the camera
    the_view->camera_position = (asteroid_radius + pc_height) * glm::vec3(1, 0, 0) + asteroid_center;
    the_view->camera_up = glm::vec3(1, 0, 0);
    the_view->camera_forward = glm::vec3(0, 1, 0);
    the_view->camera_left = glm::cross(the_view->camera_up, the_view->camera_forward);





    /*
     *  Set up buffers
     */
    auto total_vertex_count = 0;
    for (auto *artf: artf_list) {
        if (artf->the_shape->buffer_size == 0) {
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
    const int number_of_buffers = 2;
    GLuint buffers[number_of_buffers];
    glGenBuffers(number_of_buffers, buffers);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * total_vertex_count, NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(b_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_position);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 1 * total_vertex_count, NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(b_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(b_color);

    for (auto artf: artf_list) {
        if (!artf->shape_is_shared) {
            auto pos = artf->the_shape->positions();
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferSubData(GL_ARRAY_BUFFER,
                            4 * 4 * artf->the_shape->buffer_start,
                            4 * 4 * artf->the_shape->buffer_size,
                            pos->data());
            delete pos;

            auto colors = artf->the_shape->colors();
            glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
            glBufferSubData(GL_ARRAY_BUFFER,
                            4 * 1 * artf->the_shape->buffer_start,
                            4 * 1 * artf->the_shape->buffer_size,
                            colors->data());
            delete colors;
        }
    }


}


void render() {
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
    for (auto artf: artf_list) {
        artf->animate(time, delta_time);
        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(u_transform, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4fv(uColor, 1, artf->surface_material.diffuse_color.as_float().get());
        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


float lr_scale = .1f;
float fb_scale = .1f;
float ud_scale = .1f;

float lr_pan_scale = .01f;
float ud_pan_scale = lr_pan_scale;


/*
 * These variables are used to hold the window height and width and position to use when
 *  exiting full screen.
 */
static int old_width, old_height, old_xpos, old_ypos;

void general_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) {
        // only process key presses and repeats
    } else if (key == GLFW_KEY_A) {
        the_view->camera_position += lr_scale * the_view->camera_left;
    } else if (key == GLFW_KEY_D) {
        the_view->camera_position -= lr_scale * the_view->camera_left;
    } else if (key == GLFW_KEY_W) {
        the_view->camera_position += fb_scale * the_view->camera_forward;
    } else if (key == GLFW_KEY_S) {
        the_view->camera_position -= fb_scale * the_view->camera_forward;
    } else if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        if (!glfwGetWindowMonitor(window)) {
            glfwGetWindowPos(window, &old_xpos, &old_ypos);
            glfwGetWindowSize(window, &old_width, &old_height);
            auto monitor = glfwGetPrimaryMonitor();
            const auto *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(window, nullptr, old_xpos, old_ypos, old_width, old_height, 0);
        }
    } else {

    }
    //part 2 stuff
    glm::vec v = glm::normalize(the_view->camera_position - asteroid_center);
    the_view->camera_position = v * (asteroid_radius + pc_height);

    // display the position of the camera and its distance from the center
    printf("position (%f, %f, %f)      distance from center: %f     should be: %f\n",
           the_view->camera_position.x, the_view->camera_position.y, the_view->camera_position.z,
           glm::length(the_view->camera_position - asteroid_center),
           asteroid_radius + pc_height);
}


void window_size_callback(GLFWwindow *window, int width, int height) {
    auto *view = static_cast<cs4722::view *>(glfwGetWindowUserPointer(window));
    int widthfb, heightfb;
    glfwGetFramebufferSize(window, &widthfb, &heightfb);
    glViewport(0, 0, widthfb, heightfb);
    view->perspective_aspect =
            (static_cast<float>(widthfb) / static_cast<float>(heightfb));
}


void move_callback(GLFWwindow *window, double xpos, double ypos) {
    static double last_x = 0.0;
    static double last_y = 0.0;

    auto dx = xpos - last_x;
    auto dy = ypos - last_y;
    last_x = xpos;
    last_y = ypos;


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
        auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
                                                  static_cast<float>(-dx * lr_pan_scale),
                                                  glm::vec3(0, 1, 0)));
        the_view->camera_left = rot_lr * the_view->camera_left;
        the_view->camera_forward = rot_lr * the_view->camera_forward;
        the_view->camera_up = rot_lr * the_view->camera_up;
    }
}


int
main(int argc, char **argv) {
    glfwInit();

    // set the OpenGL version to use: 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    auto aspect_ratio = 1.0f;
    float ratio = 0.9f;             // ratio of window size to screen size
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
    //std::cout << xpos << " " << ypos << " " << width << " " << height << std::endl;
    int w_width = glm::min(ratio * width, aspect_ratio * ratio * height);
    int w_height = w_width / aspect_ratio;
    GLFWwindow *window = glfwCreateWindow(w_width, w_height, "User Interaction", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);


    init();
    the_view->perspective_aspect = aspect_ratio;

    glfwSetWindowUserPointer(window, the_view);
    glfwSetKeyCallback(window, general_key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);


    while (!glfwWindowShouldClose(window)) {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}


/*
 * The only change in this example from the previous one is the source of the texture image.
 * The image is fetched from a file rather than generated procedurally.
 * See the 'init_textures' function for details.
 */


#include <GLM/vec2.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "learnopengl/shader.h"
#include "cs4722/x11.h"
#include "STB//stb_image.h"
#include "../callbacks.h"

#include "reflecting_object.h"

// locations of variables in the shaders
const static auto b_position = 4;
const static auto b_texture_coord = 5;
const static auto u_sampler = 8;
const static auto u_fbsize = 9;

static const GLuint  NumVertices = 6;
static GLuint program;
static GLuint vao;
static const auto fb_texture_unit = 18;
static int texture_width, texture_height;

void init(void)
{
    /*
     * Use the same shaders as the previous example
     */
    Shader shader("vertex_shader_03A.glsl","fragment_shader_03A.glsl");
    program = shader.ID;
    glUseProgram(program);



    // generate the vertex data

    GLfloat a = 1.0f;
    GLfloat b = 1.0f;
    GLfloat c = 0.999f;
    glm::vec4 ul = glm::vec4(-b, a, c, 1);
    glm::vec4 ll = glm::vec4(-b, -b, c, 1);
    glm::vec4 lr = glm::vec4(a, -b, c, 1);
    glm::vec4 ur = glm::vec4(a, a, c, 1);

    glm::vec4 position_list[NumVertices] = { ll, ur, ul, ur, ll, lr };
    

    // generate the texture coordinate data

    auto upper_texture = 1.0f;
    auto lower_texture = 0.0f;
    auto llt = glm::vec2(lower_texture, lower_texture);
    auto ult = glm::vec2(lower_texture, upper_texture);
    auto urt = glm::vec2(upper_texture, upper_texture);
    auto lrt = glm::vec2(upper_texture, lower_texture);
    glm::vec2 texture_coordinate_list[NumVertices] = { llt, urt, ult, urt, llt, lrt };



    // Generate and bind a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    const auto number_of_buffers = 2;
    GLuint buffers[number_of_buffers];

    glGenBuffers(number_of_buffers, buffers);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * NumVertices, position_list,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_position, 4, GL_FLOAT,
                          GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_position);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * NumVertices, texture_coordinate_list,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_texture_coord, 2, GL_FLOAT,
                          GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_texture_coord);
    
}


static GLuint setup_frame_buffer()
{
    glActiveTexture(GL_TEXTURE0 + fb_texture_unit);
    GLuint texture;
    // glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // glTextureStorage2D(texture, 1, GL_RGBA8, fb_width, fb_height);
//    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, frame_buffer_width, frame_buffer_height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    GLuint fb;
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

    GLuint depth_buffer;
    glGenRenderbuffers(1, &depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texture_width, texture_height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depth_buffer);

    //  http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

    // GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    // glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    std::cout << "frame buffer complete? " <<
              glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) <<
              "(should be " << GL_FRAMEBUFFER_COMPLETE << ")" <<
              std::endl;

    return fb;
}



void render()
{
//    glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(vao);
    glUseProgram(program);
    glUniform1i(u_sampler, fb_texture_unit);
    glUniform2i(u_fbsize, texture_width, texture_height);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int
main(int argc, char** argv)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto aspect_ratio = 16.0f/9.0f;
    float ratio = 0.9f;             // ratio of window size to screen size
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
    //std::cout << xpos << " " << ypos << " " << width << " " << height << std::endl;
    texture_width = glm::min(ratio * width, aspect_ratio * ratio * height);
    texture_height = texture_width / aspect_ratio;
//    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "User Interaction", NULL, NULL);
    GLFWwindow* window = glfwCreateWindow(texture_width, texture_height, "User Interaction", NULL, NULL);
    int w_x = (width - texture_width) / 2;
    int w_y = (height - texture_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);
//    texture_width = w_width;
//    texture_height = w_height;

    gladLoadGL(glfwGetProcAddress);
//    cs4722::setup_debug_callbacks();
    auto frame_buffer = setup_frame_buffer();
    init();
    auto* the_view = reflecting_init();
    glfwSetWindowUserPointer(window, the_view);
    glfwSetKeyCallback(window, general_key_callback);
    glfwSetCursorPosCallback(window, move_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);


//    glfwSetWindowSize(window, texture_width, texture_height);
//    glfwSetWindowPos(window, w_x, w_y);
////    glfwSetWindowMonitor(window, nullptr, w_x, w_y, width, height, 0);
//    glad_glViewport(0, 0, texture_width, texture_height);

    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, texture_width, texture_height);
        reflecting_render();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, texture_width, texture_height);
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

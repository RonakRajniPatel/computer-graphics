
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

// locations of variables in the shaders
const static auto b_position = 4;
const static auto b_texture_coord = 5;
const static auto u_sampler = 8;
const static auto u_fbsize = 9;

const GLuint  NumVertices = 6;
GLuint program;

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
    glm::vec4 ul = glm::vec4(-b, a, 0, 1);
    glm::vec4 ll = glm::vec4(-b, -b, 0, 1);
    glm::vec4 lr = glm::vec4(a, -b, 0, 1);
    glm::vec4 ur = glm::vec4(a, a, 0, 1);

    glm::vec4 position_list[NumVertices] = { ll, ur, ul, ur, ll, lr };
    

    // generate the texture coordinate data

    auto upper_texture = 1.0f;
    auto lower_texture = 0.0f;
    auto llt = glm::vec2(lower_texture, upper_texture);
    auto ult = glm::vec2(lower_texture, lower_texture);
    auto urt = glm::vec2(upper_texture, lower_texture);
    auto lrt = glm::vec2(upper_texture, upper_texture);
    glm::vec2 texture_coordinate_list[NumVertices] = { llt, urt, ult, urt, llt, lrt };



    // Generate and bind a vertex array object
    GLuint vao;
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

/*
 * We use the function stbi_load to read in the data from a file and decode it
 * into a form that is usable as a texture.
 * The function returns an array of bytes storing the image data but also returns
 * information about the image: width, height, and number of 'channels'.
 *      Files with  just RGB information, no alpha, have three channels
 *      Files with RGBA information have four channels.
 *
 *  THe following variables are used to return the file information.
 */
static int texture_width, texture_height, n;

void init_texture() {

    /*
     * The function stbi_load takes five parameters
     *
     * The first parameter is a path to the file.
     *      Our examples run in the directory cmake-build-debug, that is where the
     *      executables are stored.
     *      ../media/ is a relative path to the directory storing the image files we
     *      are using in these examples
     *      The file name is added to that to complete the path to the file
     *  The second, third, and fourth parameters are the addresses of the variables
     *      set aside to hold the image information.
     *  The fifth argument is set to 0 to indicate we are not using that argument.
     *
     *  The function returns a pointer to an array of bytes (typed as
     *      unsigned char in C++ and C)
     *      that contains the data.
     *
     *      Each pixel is allocated three or four bytes in this array, depending on the
     *          value of n returned in the fourth parameter.
     *          This means that images will take a lot more space in memory than they
     *          do on disk (unless reading in a TIFF or old BMP file which were uncompressed)
     *
     *      It is a good idea to delete the array once we get the data into the texture.
     */
    unsigned char *texture_data = stbi_load("../media/729144-real-myst-windows-screenshot-beautiful-detail-in-the-graphics.png",
        &texture_width, &texture_height, &n, 0);
    printf("%d x %d    %d\n", texture_width, texture_height, n);  // debugging


    /*
     * The remainder of setting up the texture is the same as in the previous example.
     */
    GLuint texture;
    glGenTextures(1, &texture);
    /*
     * Although the constants GL_TEXTURE0 and so on are numbers with no particular meaning,
     * they are related to each other.
     * The following trick may not be all that useful, but it does work.
     */
    glActiveTexture(GL_TEXTURE0 + 3);  // GL_TEXTURE3 is the same
    glBindTexture(GL_TEXTURE_2D, texture);

    auto number_of_levels = 1;
    auto internal_format = GL_RGBA8;
    auto external_format = (n == 3) ? GL_RGB : GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, texture_width, texture_height, 0, external_format,
                 GL_UNSIGNED_BYTE, texture_data);
    /*
     * Delete the texture data.
     * The [] indicates that we are deleting an array.
     */
    delete[] texture_data;
  

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, cs4722::x11::aquamarine.as_float().get());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    auto mag_filter = GL_NEAREST;
    auto mag_filter = GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
     auto wrap_type = GL_CLAMP_TO_EDGE;
//    auto wrap_type = GL_MIRRORED_REPEAT;
//    auto wrap_type = GL_REPEAT;
//    auto wrap_type = GL_CLAMP_TO_BORDER;
//    auto wrap_type = GL_MIRROR_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_type);

    glUniform1i(u_sampler, 3);

}


void
display(void)
{
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

    auto aspect_ratio = 1.0f;
    float ratio = 0.9f;             // ratio of window size to screen size
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(primary, &xpos, &ypos, &width, &height);
    //std::cout << xpos << " " << ypos << " " << width << " " << height << std::endl;
    int w_width = glm::min(ratio * width, aspect_ratio * ratio * height);
    int w_height = w_width / aspect_ratio;
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "User Interaction", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);
//    cs4722::setup_debug_callbacks();
    init();
    init_texture();

    glfwSetWindowSize(window, texture_width, texture_height);
//    glfwSetWindowMonitor(window, nullptr, w_x, w_y, width, height, 0);
    glad_glViewport(0, 0, texture_width, texture_height);

    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float().get());
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}


/*
 * We can create a skybox by putting our scene inside a cube and then texturing the cube using a cube map.
 * The cube should be large enough to contain the scene.
 * Making the cube much larger will help prevent the camera from 'escaping' (as Truman did in a rowboat).
 */

 #include <GLM/gtc/type_ptr.hpp>
#include <glad/gl.h>

#include "learnopengl/shader.h"
#include "STB/stb_image.h"

#include "cs4722/view.h"
#include "cs4722/artifact.h"
#include "cs4722/texture_utilities.h"
#include "../callbacks.h"


static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact*> artifact_list;

static GLuint b_position = 1;
static GLuint b_texture_coordinates = 2;

static GLuint u_transform = 1;
static GLuint u_sampler2 = 2;
static GLuint u_samplerC = 3;
static GLuint u_surface_effect = 4;

static GLuint vao;


void init()
{

    the_view = new cs4722::view();
    the_view->enable_logging = 0;
    the_view->set_flup(glm::vec3(0.202971,-0.227978,-0.952277),
                       glm::vec3(-0.978031,0,-0.20846),
                       glm::vec3(0.0475242,0.973667,-0.222969),
                       glm::vec3(-0.146291,0.195185,1.3214));



//    printf("ready to compile\n");
    Shader shader("vertex_shader01.glsl","fragment_shader01.glsl");
    program = shader.ID;
    glUseProgram(program);


    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);


    cs4722::init_texture_from_file("../media/tulips-bed-2048x2048.png", 2);
    cs4722::init_texture_computed(1, 8);


    /*  This next section of code will initialize and populate a cube texture.
     *  We will use the set up from the web site https://www.humus.name/index.php?page=Textures.
     *  A directory holds six image files for a cube map.
     *  The files are named posx, negx, posy, negy, posz, and negz.
     *  These correspond to the X+ face, the X- face, the Y+ face, and so on.
     *  The utility we are using for loading image files will handle jpg and png files.
     *
     */

    // Each pair of lines out of the next six lines gives a path to a folder containing image files
    //      and the extension for those files.
    // Uncomment a pair of lines (and make usre the other 4 are commented) to see alternative cube maps.
    auto base_path = "../media/fjaderholmarna";
    auto image_extension = "png";
//    auto base_path = "../media/oriented-cube-reversed";
//    auto image_extension = "png";
//    auto base_path = "../media/blue_cloud";
//    auto image_extension = "jpg";

    // the next definitions are to help create the path to each file
    auto file_names = {"posx", "negx", "posy", "negy", "posz", "negz"};
    auto blen = strlen(base_path);
    auto elen = strlen(image_extension);
    char posx[blen+elen+6];

    // create a texture
    GLuint cube_texture;
    glGenTextures(1, &cube_texture);
    //  Bind to texture unit 10
    //  This is the old way of binding, but is the way it must be done for cube textures
    //      at least for OpenGL implemented on my systems
    glActiveTexture(GL_TEXTURE10);
    // Bind the texture to a binding point.
    // This is similar to how vertex buffer data was managed.
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);


    const auto internal_format = GL_RGBA8;

    /*
     * Each image will be loaded into the corresponding face of the cube texture.
     * Each face is identified by a symbolic constant.
     * Conveniently the constants are sequential integers, so we can step through the constants
     *      by starting with the first constant and adding repeatedly adding 1 to get the other
     *      faces.
     * The first constant is for the positive X face, X+.
     * The other faces, in order, are: X-, Y+, Y-, Z+, and Z-.
     * Notice that the image paths are specified in the same order.
     */

    // face starts with the symbolic constant indicate the X+ face.
    // The other faces are consecutive integers after this one.
    auto face = GL_TEXTURE_CUBE_MAP_POSITIVE_X ;

    // loop over each file name: posx, negx, ....
    // the faces of the cube map are loaded one at a time
    for(auto const name : file_names)
    {
        // create the path to the file
        strcpy(posx, base_path);
        strcpy(posx + blen, "/");
        strcpy(posx+blen+1, name);
        strcpy(posx+blen+5,".");
        strcpy(posx+blen+6,image_extension);
        *(posx+blen+6+elen) = '\0';
//        printf("file name %s\n", posx);


        // three variables to receive attributes of the image that is loaded
        // n is the number of components in each pixel
        //      we assume that n = 4 means RGBA and that n = 3 means RGB
        int texture_width, texture_height, n;
        // read the file and return a pointer to the data.
        auto *texture_data = stbi_load(posx,
                                       &texture_width, &texture_height, &n, 0);
//        printf("after input %d  %d %d %d\n", texture_data, texture_width, texture_height, n);
        const auto external_format = (n == 3) ? GL_RGB : GL_RGBA;
        // put the data into one face
        glTexImage2D(face,
                     0, // level
                     internal_format,
                     texture_width, texture_height,   // taken from the image
                     0, // border
                     external_format,
                     GL_UNSIGNED_BYTE,  // component type of the external image
                     texture_data
        );
        // the image data is no longer needed in the driver, it is now in the texture
        delete texture_data;
        // increment the face for the next iteration.
        face++;
    }

    // set parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // the cube texture is 3D, so there are three coordinates
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);





    /*
     * First put in two objects to have some kind of scene.
     */
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

    /*
     * Now create a cube to hold the 'background'
     */

    /*
     * This variable holds the size of the skybox.
     * Making it very large means that, practically speaking, the camera will not escape from
     *      the cube that displays the texture.
     */
    auto skybox_scale = 40000.0f;

    /*
     * Other shapes can be used, but a torus will have a strange effect.
     */
    auto* a_shape = new cs4722::block();
//     auto* a_shape = new cs4722::sphere();
//     auto* a_shape = new cs4722::cylinder();
//     auto* a_shape = new cs4722::torus();

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
    glVertexAttribPointer(b_texture_coordinates, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_texture_coordinates);

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Skybox", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);;

    gladLoadGL(glfwGetProcAddress);

    init();
    the_view->perspective_aspect = aspect_ratio;

//    printf("view logging %d\n", the_view->enable_logging);

    glfwSetWindowUserPointer(window, the_view);
    glfwSetKeyCallback(window, general_key_callback);
    glfwSetCursorPosCallback(window, move_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
	
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

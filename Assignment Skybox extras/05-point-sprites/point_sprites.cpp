
/**
 *  We can implement a form of sprites by painting textures onto the images created for POINT primitives.
 *  Recall that points are drawn as squares.
 *  The fragment shader is called for each pixel in the square.
 *  So, if the point size is set to 100, then there will be 10,000 calls to the fragment shader for that point.
 *
 *  The mechanisms we have been using to paint textures will not work here since the point is given by
 *      one vertex so will only have one set of texture coordinates.
 *  However, the GLSL language provides a predefined variable gl_PointCoord that contains the relative coordinates
 *      of a fragment within the overall point.
 *  According to OpenGL documentation use of gl_PointCoord should be enabled by default. However, it seems
 *      not to be.  The function call to enable is near the beginning of function init()
 *
 *
 *  The file point-shape.h defines a 'shape' class that consists of a set of points.
 *  See that file for details.
 *
 *  These point sprites always face the camera.
 *
 *
 */




#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/random.hpp>


#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "learnopengl/shader.h"

#include "cs4722/artifact.h"
#include "cs4722/view.h"
#include "cs4722/texture_utilities.h"
#include "point-shape.h"
#include "../callbacks.h"

static GLuint program;
static auto* the_view = new cs4722::view();
static std::vector<cs4722::artifact*> parts_list;
GLuint  vao;

static auto b_position = 1;
static auto b_color = 2;
static GLuint u_transform = 1;
static GLuint u_sampler = 2;
static GLuint u_surface_effect = 3;


void init()
{

    Shader shader("vertex_shader05.glsl","fragment_shader05.glsl");
    program = shader.ID;
	glUseProgram(program);

    glPointSize(50);
    /**
     * The following statement is needed in some circumstances
     * to enable using gl_PointCoord in the fragment shader.
     *
     * This does not seem to be needed now.
     * Possibly because the extension GL_ARB_point_sprite was added to GLAD when configuring it.
     *
     * However if sprites do not appear, try uncommenting the next line to see if that works.
     */
//    glEnable(34913);
//    glEnable(GL_POINT_SPRITE_ARB); // symbolic constant for 34913
                                    // but doesn't seem to be defined

/**
 * The images used for sprites in this example do not have alpha less than one so there will be no
 * transparency effects.
 * See the next example for how this works.
 */
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	the_view->set_flup(glm::vec3(-0.352275, -7.45058e-09, -0.935897),
		glm::vec3(-0.935897, 0, 0.352274),
		glm::vec3(5.85944e-08, 1, 1.20934e-08),
		glm::vec3(1.34801, 0.785008, 3.00568));

	auto num_groups = 15;
	auto num_points_per_group = 3;
	auto min_c = -2.0;
	auto max_c = 2.0;

	for (int group = 0; group < num_groups; group++)
	{

		auto* b = new points_shape();

		for (int i = 0; i < num_points_per_group; i++)
		{
			auto x = glm::linearRand(min_c, max_c);
			auto y = glm::linearRand(min_c, max_c);
			auto z = glm::linearRand(min_c, max_c);
			b->position_list->push_back(glm::vec4(x, y, z, 1));
		}

		// auto* obj = new object_random_walk();
		auto* part = new cs4722::artifact_rotating();

		
		auto cx = glm::linearRand(min_c, max_c);
		auto cy = glm::linearRand(min_c, max_c);
		auto cz = glm::linearRand(min_c, max_c);
        part->animation_transform.rotation_center = glm::vec3(cx, cy, cz);

        part->the_shape = b;
        part->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
        part->rotation_rate = M_PI / 3;
        part->texture_unit = group % 3;
        part->surface_effect = 1;
		parts_list.push_back(part);
	}


    cs4722::artifact_rotating* artf = new cs4722::artifact_rotating();
    artf->the_shape = new cs4722::torus();
    artf->world_transform.translate = the_view->camera_position + 7.0f * the_view->camera_forward;
    artf->surface_effect = 2;
    parts_list.push_back(artf);


	auto pl = parts_list;



    auto total_vertex_count = 0;
    for(auto *artf : parts_list) {
        if(artf->the_shape->buffer_size == 0) {
            artf->the_shape->buffer_start = total_vertex_count;
            artf->the_shape->buffer_size = artf->the_shape->get_size();
            total_vertex_count += artf->the_shape->get_size();
        } else {
            artf->shape_is_shared = true;
        }
    }

//    GLuint vao;
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
    glBufferData(GL_ARRAY_BUFFER, 4 * 1 * total_vertex_count, nullptr,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(b_color);



    for (auto artf : parts_list) {
        if(!artf->shape_is_shared) {
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



void display()
{

    glBindVertexArray(vao);
    glUseProgram(program);

    static auto last_time = 0.0;
    auto *tv = the_view;

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto obj : parts_list) {
         obj->animate(time, delta_time);

        auto model_transform = obj->animation_transform.matrix() * obj->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(u_transform, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform1i(u_sampler, obj->texture_unit);
        glUniform1i(u_surface_effect, obj->surface_effect);


        glDrawArrays(obj->the_shape->drawing_mode, obj->the_shape->buffer_start, obj->the_shape->buffer_size);
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
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Point Sprites", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);
	// gladLoadGL();
//	cs4722::setup_debug_callbacks();
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL renderer " << glGetString(GL_RENDERER) << std::endl;


    the_view->set_camera_position(glm::vec3(0, 0, 1));

	
	init();
    the_view->perspective_aspect = aspect_ratio;

    cs4722::init_texture_from_file("../media/sprite.png", 1);
    cs4722::init_texture_from_file("../media/sprite2.png", 2);
    cs4722::init_texture_computed(0);


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
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}

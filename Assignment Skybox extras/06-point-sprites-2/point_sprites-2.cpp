
/**
 * This example is from the OpenGL programming guide.
 * The setup is the same as the first point-sprite example.
 * The main change is that the pattern on the point is computed in the fragment shader.
 *      So, no textures are created.
 *
 *  This example also demonstrates blending colors.  This makes the sprites semi-transparent.
 *  See the comments near the beginning of the init() function for a discussion.  Also check the fragment shader.
 */

#include <GLM/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <glad/gl.h>


#include <GLFW/glfw3.h>

#include "learnopengl/shader.h"

#include "cs4722/view.h"
#include "cs4722/artifact.h"
#include "cs4722/texture_utilities.h"
#include "point-shape.h"
#include "../callbacks.h"

static GLuint program;
static auto* the_view = new cs4722::view();
static std::vector<cs4722::artifact*> parts_list;
GLuint  vao;

static auto b_position = 1;
static GLuint u_transform = 1;



void init()
{

    Shader shader("vertex_shader06.glsl","fragment_shader06.glsl");
    program = shader.ID;
	glUseProgram(program);


    glPointSize(180);
    // the following is needed to enable point sprites.
//    glEnable(GL_POINT_SPRITE_ARB);  // 34913
//    glEnable(34913);


    // the texture used has transparency so sprites can be partially visible even if behind other sprites.
	glEnable(GL_BLEND);
	// Blending can be done in many ways.
	// This way uses the alpha channel of the value assigned to fColor
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*
	 * Blending can be complex if mixed with depth testing.
	 * A fragment that is behind another fragment but is computed later will be ignored under
	 * depth testing, even if it should be partially visible.
	 *
	 * You will notice some anomalies, shapes 'popping', when these conflicts arise dynamically.
	 *
	 * One solution is, even using depth testing, to render primitives from back to front
	 */
	glEnable(GL_DEPTH_TEST);


	the_view->set_flup(glm::vec3(-0.352275, -7.45058e-09, -0.935897),
		glm::vec3(-0.935897, 0, 0.352274),
		glm::vec3(5.85944e-08, 1, 1.20934e-08),
		glm::vec3(1.34801, 0.785008, 3.00568));

	auto num_groups = 15;
	auto num_points_per_group = 3;
	auto min_c = -4.0;
	auto max_c = 4.0;

	const auto z_base = -5.0;
	const auto z_delta = .1;
    auto z = z_base;

	for (int group = 0; group < num_groups; group++)
	{

		auto* b = new points_shape();

		for (int i = 0; i < num_points_per_group; i++)
		{
			auto x = glm::linearRand(min_c, max_c);
			auto y = glm::linearRand(min_c, max_c);
//			auto z = glm::linearRand(min_c, max_c);
			b->position_list->push_back(glm::vec4(x, y, z, 1));
		}

		// auto* obj = new object_random_walk();
		auto* obj = new cs4722::artifact_rotating();

		
		auto cx = glm::linearRand(min_c, max_c);
		auto cy = glm::linearRand(min_c, max_c);
//		auto cz = glm::linearRand(min_c, max_c);
		auto cz = z;
		obj->animation_transform.rotation_center = glm::vec3(cx, cy, cz);

		obj->the_shape = b;
		obj->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
		obj->rotation_rate = M_PI / 3;
		obj->texture_unit = group % 3;
		parts_list.push_back(obj);

		z -= z_delta;
	}

//	auto pl = parts_list;   // used to make parts_list visible during debugging



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
    const int number_of_buffers = 1;
    GLuint  buffers[number_of_buffers];
    glGenBuffers(number_of_buffers, buffers);


    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * total_vertex_count, NULL,  GL_STATIC_DRAW);
    glVertexAttribPointer(b_position, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(b_position);

    for (auto artf : parts_list) {
        if(!artf->shape_is_shared) {
            auto pos = artf->the_shape->positions();
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBufferSubData(GL_ARRAY_BUFFER,
                            4 * 4 * artf->the_shape->buffer_start,
                            4 * 4 * artf->the_shape->buffer_size,
                            pos->data());
            delete pos;

        }
    }
}



void display()
{

    glBindVertexArray(vao);
    glUseProgram(program);

    static auto last_time = 0.0;
//    auto *tv = the_view;  /// used to make the_view visible during debugging

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

        glDrawArrays(obj->the_shape->drawing_mode, obj->the_shape->buffer_start, obj->the_shape->buffer_size);
    }
}


//int main(int argc, char** argv)
int main()
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
    GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Point Sprites 2", NULL, NULL);
    int w_x = (width - w_width) / 2;
    int w_y = (height - w_height) / 2;
    glfwSetWindowPos(window, w_x, w_y);
    glfwMakeContextCurrent(window);


    gladLoadGL(glfwGetProcAddress);
//	cs4722::setup_debug_callbacks();

	the_view->set_camera_position(glm::vec3(0, 0, 1));
    the_view->enable_logging = false;
	
	init();

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

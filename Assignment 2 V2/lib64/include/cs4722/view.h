#pragma once

#include <GLFW/glfw3.h>

#include "GLM/mat4x4.hpp"
#include "GLM/ext/matrix_clip_space.hpp"
#include "utility_glm.h"

namespace cs4722
{


	class view
	{
	public:

	    /**
	     * \brief Initialize the perspective and camera.
	     *

	        Attribute | Initial Value
	        ----- | -----
	        `perspective_near` | `.1f`
			`perspective_fovy` | `M_PI / 2`
			`perspective_aspect` | `1.0f`
			`camera_forward` | `glm::vec3(0, 0, -1)`
			`camera_up` | `glm::vec3(0, 1, 0)`
			`camera_position` | `glm::vec3(0, 0, 0)`
			`camera_left` | `glm::cross(camera_up, camera_forward)`
	     *
	     *
	     *
	     */
		view() :
			perspective_near(.1f),
			perspective_fovy(M_PI / 2),
			perspective_aspect(1.0f),
			camera_forward(glm::vec3(0, 0, -1)),
			camera_up(glm::vec3(0, 1, 0)),
			camera_left(glm::cross(camera_up, camera_forward)),
			camera_position(glm::vec3(0, 0, 0))
		{}




		/**
		 *  \brief
			Set the camera position, leaving the camera orientation unchanged.
		*/
		void set_camera_position(glm::vec3 const position);


		/**
		 * \brief
		 * Reverse the direction in which the camera is pointing.
		 *
		 * This function reverses the left vector but leaves the up vector fixed.
		 *
		 * The function was included only to support one example illustrating rendering to a buffer.
		 *
		 */
		void reverse_camera();

		/**
		 * \brief
		 * Set the four camera attributes.
		 *
		 * The output when logging is enabled in the view is formatted to be easily used
		 * in this function.
		 * A line of logging output can be copied and pasted in as the parameters to a function call.
		 * All necessary `vec3` calls and commas are provided.
		 *
		 * 'flup' is a mnemonic for the order of the four parameters.
		 *
		 * @param forward  Should be a unit vector.  Should be orthogonal to left and up.
		 * @param left Should be a unit vector.  Should be orthogonal to forward and up.
		 * @param up  Should be a unit vector.  Should be orthogonal to forward and left.
		 * @param position  No restrictions on position
		 */
		void set_flup(glm::vec3 forward, glm::vec3 left, glm::vec3 up,
			glm::vec3 position);


		

		float perspective_near;  ///< The near parameter of the perspective projection
		float perspective_fovy; ///< The field of view of the perspective projection, measured in radians
		float perspective_aspect;  ///< The aspect ratio of the perspective projection

		/**
		 * \brief Direction towards which the camera is pointing
		 *
		 * Should be a unit vector and orthogonal to `camera_up` and `camera_left`.
		 * `camera_left` should equal to `camera_up` crossed with `camera_forward`.
		 */
		glm::vec3 camera_forward;
        /**
         * \brief Points up relative to the camera.
         *
         * Should be a unit vector and orthogonal to `camera_forward` and `camera_left`.
         * `camera_left` should equal to `camera_up` crossed with `camera_forward`.
         */
		glm::vec3 camera_up;
        /**
         * \brief Points to the left of the camera.
         *
         * Should be a unit vector and orthogonal to `camera_up` and `camera_forward`.
         * `camera_left` should equal to `camera_up` crossed with `camera_forward`.
         */
        glm::vec3 camera_left;
        /**
         * \brief Location of the center of the camera.
         */
		glm::vec3 camera_position;



//		double last_x = 0.0; ///< Last known location of the mouse, x coordinate
//		double last_y = 0.0; ///< Last known location of the mouse, y coordinate
//
		/**
		 * \brief Display the camera attributes, position and orientation.
		 *
		 * The format is designed to allow directly pasting the parameters
		 * into a call of `set_flup`.
		 */
		void display_parameters() const;

		/**
		 * \brief Set to true to enable the various callback functions to call `display_parameters`.
		 *
		 */
		bool enable_logging;
	};



}

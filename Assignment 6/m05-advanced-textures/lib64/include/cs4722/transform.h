//
// Created by Ben on 11/6/2020.
//

#pragma once

#include "GLM/vec3.hpp"
#include "GLM/mat4x4.hpp"
#include "GLM/gtc/matrix_transform.hpp"

namespace cs4722 {

    /**
     * \brief Represents an affine transformation.
     *
     * This class represents a transformation as the product of a scale transformation followed by
     * a rotation transformation followed by a translate transformation.
     * The parameters for each of those component transformations are stored as attributes.
     */
    class transform {
    public:

        /**
         * \brief The axis of rotation will go through this point.
         *
         *  The point should be in 'object' coordinates.
         *
         */
        glm::vec3 rotation_center = glm::vec3(0, 0, 0);
        /**
         * \brief A vector indicating the direction of the axis of rotation.
         *
         * A positive angle of rotation will be
         * counter-clockwise when the rotation axis is pointing towards the view.
         */
        glm::vec3 rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);
        /**
         * \brief Amount of rotation, measured in radians.
         *
         * A positive angle of rotation will be
         * counter-clockwise when the rotation axis is pointing towards the view.
         *
         */
        float rotation_angle = 0.0;
        /**
         * \brief Translation values in each axis direction.
         */
        glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
        /**
         * \brief Scale values in each axis direction.
         *
         * Note that the default values are all 1.0.
         */
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        /**
         * \brief Compute a matrix representing this transformation.
         *
         * The matrix is equivalent to applying the following transformations in
         * this order:
         *
         * -# Scale
         * -# Translate by -scale*rotation_center
         *      * This puts the center of rotation at the origin
         * -# Rotate
         *      * The functions provided in the GLM assume the axis of rotation goes through the origin.
         * -# Translate by scale*rotation_center
         * -# Translate using the `translate` attribute
         *
         * @return a matrix representing this transformation
         */
        glm::mat4 matrix();

//        {
//            glm::mat4 other = glm::mat4(1.0f); // identity
//            other = glm::translate(other, translate);
//            other = glm::translate(other, rotation_center*scale);
//            other = glm::rotate(other, rotation_angle, rotation_axis);
//            other = glm::translate(other, -rotation_center*scale);
//            other = glm::scale(other, scale);
//            return other;
//        }

    };


}
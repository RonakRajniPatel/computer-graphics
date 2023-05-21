//
// Created by bsetz on 4/7/2021.
//

#ifndef ASSIGNMENT04_PART_ELLIPTICAL_ORBIT_H
#define ASSIGNMENT04_PART_ELLIPTICAL_ORBIT_H

#include "cs4722/artifact.h"
#include "GLM/vec4.hpp"

namespace cs4722 {

    /** \brief A class which represents a `part` that can rotate on an axis and also move in an elliptical path.
     *
     *
     *
     */
    class artifact_elliptical_orbit : public cs4722::artifact_rotating {

    public:
        /** \brief A vector pointing from the center of the ellipse to a farthest point on the orbit.
         *
         *  There are actually two furthest points on the ellipse, this vector points to one of them.
         *
         *  The length of this vector is half the major axis of the ellipse, also known as the semi-major axis.
         *
         *  `aphelion_vector` should be perpendicular to `perihelion_vector`.
         */
        glm::vec3 aphelion_vector = glm::vec3(1,0,0);
        /** \brief A vector pointing from the center of the ellipse to a nearest point on the orbit.
         *
         *  There are actually two nearest points on the ellipse, this vector points to one of them.
         *
         *  The length of this vector is half the minor axis of the ellipse, also known as the semi-minor axis.
         *
         *  `aphelion_vector` should be perpendicular to `perihelion_vector`.
         */
        glm::vec3 perihelion_vector = glm::vec3(0, 0, 1);

        /** \brief  The center of the ellipse.
         *
         * The ellipse is symmetrical about its center.
         * Don't confuse the center with the foci (the two focus points).
         *
         */
        glm::vec3 path_center = glm::vec3(0,0,0);

        /** \brief The rate in radians per second at which the part moves around the orbit.
         *
         * This is not consistent with physics.
         * In motion constrained by gravity, bodies will move faster when closer together and slower when
         * farther apart.
         * However, this is good enough for our purposes.
         *
         * The period of the orbit is 2*&pi;/orbital_rate.
         *
         */
        double orbital_rate = 0.0;  // radians per second

        /** \brief Added to the clock time when computing the position of the part in the orbit.
         *
         *  Including this value allows setting the starting point of the part to something other than t = 0.
         *  A value between 0 and the orbital period is sufficient to cover any possible starting position.
         *
         */
        double time_offset = 0.0;

        /** \brief Initialize the part with default values.
         *
         *
         *
         *
         *
            Attribute | Default Value
            --------- | ------------
            aphelion_vector | vec3(1,0,0)
            perihelion_vector | vec3(0, 0, 1)
            path_center | vec3(0,0,0)
            orbital_rate | 0.0
            time_offset | 0.0
         *
         */

        artifact_elliptical_orbit();

        void animate(double time, double delta_time) override;


    };
}
#endif //ASSIGNMENT04_PART_ELLIPTICAL_ORBIT_H





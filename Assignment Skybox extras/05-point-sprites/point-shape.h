//
// Created by bsetz on 10/7/2021.
//

#pragma once

/*
 * A shape consisting of a set of points.
 */
class points_shape : public cs4722::shape
{
public:

    /**
     * Store a list of positions of the points
     */
    std::vector<glm::vec4> *position_list;

    /**
     *  Initialize the list of points to empty.
     *  Also set the drawing mode to POINTS
     */
    inline points_shape()
            : position_list(new std::vector<glm::vec4>())
    {
        drawing_mode = GL_POINTS;
    }

    /**
     * Return the list of positions.
     * @return
     */
    inline std::vector<glm::vec4>* positions() override
    {
        return position_list;
    }

    /**
     * Return an empty list
     */
    inline std::vector<cs4722::color>* colors() override
    {
        return new std::vector<cs4722::color>();
    }
    /**
     * Return an empty list
     */

    inline std::vector<glm::vec2>* texture_coordinates() override
    {
        return new std::vector<glm::vec2>();
    }

    /**
     * Return an empty list
     */
    inline std::vector<glm::vec4>* normals() override
    {
        return new std::vector<glm::vec4>();
    }

    /**
     * The number of positions is the number of vertices for this 'shape'.
     */
    inline int get_size() override
    {
        return position_list->size();
    }

};

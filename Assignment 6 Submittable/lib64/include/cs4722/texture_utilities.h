//
// Created by Ben on 11/11/2020.
//

#pragma once

#include <glad/gl.h>
#include <vector>
#include <string>
#include "cs4722/x11.h"

namespace cs4722 {

    /**
     * \brief
     * Create a texture from an image file and assign it to a designated texture unit.
     *
     * The image is loaded from a file and a texture is created from the image data.
     * This function cleans up the temporary data areas used in the process.
     *
     * @param path      Path to the image file
     * @param texture_unit  Texture unit to use for this texture
     * @returns   The texture identifier
     */
    GLuint init_texture_from_file(const char *path, const GLuint texture_unit);

    //GLuint init_textures_from_files(std::vector<std::string> paths, const GLuint start_unit);


    /**
     * \brief create a simple gradient texture.
     *
     * A simple texture is created as a gradient of two colors.
     *
     * @param texture_unit  The texture unit number to use for the texture
     * @param texture_size  Texture size in pixels, default 16
     * @param color1  One color to use in the gradient, default yellow
     * @param color2 Other color to use in the gradient, default blue
     * @returns   The texture identifier
     *
     */
    GLuint init_texture_computed(int texture_unit, int texture_size = 16,
                               cs4722::color color1 = cs4722::x11::yellow,
                               cs4722::color color2 = cs4722::x11::blue);

    /**
     * \brief Create a cube texture from a list of paths to six image files for the six sides
     * of the cube.
     *
     *
     * @param paths List of six paths to image files
     * @param environment_unit   The texture unit number to use for the cube texture
     * @returns   The texture identifier
     */
    GLuint init_cube_texture_from_file(std::vector<std::string> &paths, int environment_unit);

    /**
     * \brief Create a cube texture from the files located in the directory indicated by `base_path`.
     *
     * Create a cube texture from the files located in the directory indicated by `base_path`.
     * It is assumed that the files are named posx.*, negx.*, posy.*, ...
     *
     * The base path should not end with '/' or '\', the separator will be added by the function.
     * The optional extension should not have a period, the separator will be added by the function.
     *
     * @param base_path Path to folder in which the six files are located.
     *          Do not include the trailing / or \ in this value.
     * @param environment_unit Texture unit to use for this texture.
     * @param ext Extension to add to the file name, defaults to png.
     *          Do not include the period in this value.
     * @returns   The texture identifier
     */
    GLuint init_cube_texture_from_path(const char* base_path, int environment_unit, const char* ext = "png");

}
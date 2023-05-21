//
// Created by Ben on 12/10/2020.
//
#pragma once

#include "cs4722/x11.h"

namespace cs4722 {

    class light {
    public:
        color ambient_light = x11::gray50;
        color diffuse_light = x11::white;
        color specular_light = x11::white;

        glm::vec4 light_direction_position = glm::vec4(0.0, -1.0, 0.0, 0.0);
        bool is_directional = true;

        double shininess = 20.0;
        double specular_strength = 1.0;


    };
}
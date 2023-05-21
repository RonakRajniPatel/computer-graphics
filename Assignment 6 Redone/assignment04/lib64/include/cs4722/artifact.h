

#pragma once


#include <list>
#include <GLM/vec3.hpp>
//#include <GLM/ext.hpp>
#include "cs4722/x11.h"
#include "cs4722/shape.h"
#include "cs4722/transform.h"


namespace cs4722
{


    /**
     * \brief Represents common characteristics of surfaces for use in lighting
     *  and shading.
     *
     *
     */
    class material {
    public:
        cs4722::color diffuse_color = cs4722::x11::steel_blue,
            ambient_color = cs4722::x11::gray50,
            specular_color = cs4722::x11::olive_drab;
        double shininess = 20.0;
        float specular_strength = 1.0;
        bool reflecting = false;
    };


    /**
     * \brief Represents a geometric object based on a standard shape.
     *
     * An `object` instance 
     */
    class artifact {
    public:

        

        virtual void animate(double time, double delta_time)
        {
        }



        material surface_material;

        transform world_transform;
        transform animation_transform;
        shape* the_shape = nullptr;
        bool shape_is_shared = false;
        int texture_unit = 0;
        int surface_effect = 0;

        artifact* parent = nullptr;
        std::list<artifact*> children;


    	
    };

    class artifact_rotating : public artifact
    {

    public:
		/**
			\brief Rotation rate in radians per second.
			
			For example, to rotate once per second, the rate should be 2*&pi;.
			To rotate once very three seconds, the rate should be 2*&pi;/3.
			To rotate three times every second, the rate should be 2*&pi;*3.
			
		*/
        float rotation_rate = 0.0f;  // radians per second
        float rotation_offset = 0.0f;


    	void animate(const double time, const double delta_time) override
        {
            animation_transform.rotation_angle = (static_cast<float>(time) * rotation_rate) + rotation_offset;
        }


        
    };

}
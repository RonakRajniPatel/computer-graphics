#pragma once


 #include <GLFW/glfw3.h>
// #include "vmath.h"
// #include <algorithm>
// #include <iostream>
#include "GLM/vec4.hpp"
#include "GLM/vec2.hpp"
#include <vector>
#include "cs4722/x11.h"

namespace cs4722 {

    /**
\brief Base class of classes that represent particular geometric shapes such as
rectangular parallelepipeds or spheres

The class shape is the base class for classes representing geometric shapes.
These shapes generally have a fixed size and orientation.
The intention is that transformations will be applied to place and size
these shapes appropriately.
This is the purpose of the
[artifact](@ref artifact) class.

Subclasses will implement several methods that provide vertex data for
a shape: positions, normal vectors, texture coordinates, and tangent vectors.
Subclasses will also provide a color pattern that is useful in basic examples.
The root class `shape` has, at this point, six subclasses.

      
- [block](@ref block)
  - A unit cube centered at the origin
- [sphere](@ref sphere)
  - A unit sphere centered at the origin
- [cylinder](@ref cylinder)
  - A cylinder of height 1 and radius 1 centered at the origin with central
      axis along the y axis
- [torus](@ref torus)
  - A torus surrounding the z axis with outer radius 1
- [torus_points](@ref torus_points)
  - Just the points created by a triangulation of a torus
- [rectangle](@ref rectangle)
  - A square


## Usage

The  way the shape classes should be used is as follows:

* Create a shape with whatever parameters desired for the appropriate constructor
* Get the vertex lists needed for a particular application and put them into
    a buffer
    * This will involve setting `buffer_start` and `buffer_size` but should
         not change any other attributes
    * Once the lists have been used to initialize buffers, they can be deleted
* Do not change attributes of the `shape` after this




     */




	class shape
	{
	public:



		virtual ~shape();

		/**
		 * \brief Returns the number of vertices in the description of a shape.
		 * @return  The number of vertices in a shape.
		 */
		virtual int get_size() = 0;

		/**
		 * \brief Returns the positions of the vertices of primitives making
		 * up a shape.
		 *
		 * The positions are `vec4`s with fourth coordinate set to 1.
		 *
		 * @return Positions of vertices of primitives making up the shape.
		 */
		virtual std::vector<glm::vec4>* positions() = 0;

		

		/**
		 * \brief Returns a list of colors for each vertex.
		 *
		 * The intent is to provide a simple way to color a shape in
		 * such a way as to give it some three dimensional feel.
		 * For most of the shapes this is achieved by coloring each triangle with
		 * a solid color and alternating colors between triangles as much
		 * as possible.
		 *
		 * The `color_set_` attribute provides the list of colors used in
		 * the coloring.
		 * `color_set_` is given a default value.
		 * If changing it, provide a list of at least two colors to get the
		 * three dimensional effect.
		 *
		 *
		 * @return A list of colors for each vertex.
		 */
		virtual std::vector<cs4722::color>* colors() = 0;

		/**
		 * \brief An alternative coloring scheme for vertices.
		 *
		 * @return a list of colors for each vertex.
		 */
		std::vector<cs4722::color>* mixed_colors();

		/**
		 * \brief Provides normal vectors for each vertex.
		 *
		 * Vectors are represented as `vec4`s with fourth coordinate 0.
		 *
		 * @return A list of normal vectors for each vertex.
		 */
		virtual std::vector<glm::vec4>* normals() = 0;


        /**
         * \brief Returns a tangent vector for each vertex.
         *
         * In order to utilize bump mapping, a coordinate frame is required at each
         * point on the surface of a shape.
         * This function provides one tangent vector.
         * Another independent one can be obtained with a cross product with
         * the normal vector.
         *
         * It is important that these tangent vectors be consistent over the surface.
         * This is possible for some surfaces, such as the torus.
         * However, some surfaces, such as a sphere, must have some 'whorls'.
         *
         * Unlike the other functions returning geometric characteristics for vertices, this
         * one is implemented in `shape` to return an empty list.
         * At the moment the function is not implemented in all subclasses.
         *
         * @return
         */
		virtual std::vector<glm::vec4>* tangents();


		/**
		 * \brief Return texture coordinates assigned to each verte.
		 *
		 *
		 * @return  A list of texture coordinate for the vertices defining this shape.
		 */
		virtual std::vector<glm::vec2>* texture_coordinates() = 0;


		/**
		 * \brief  Set the color set
		 *
		 * This is obsolete since `color_set_` is publi.
		 *
		 * @param new_color_set
		 */
		void set_color_set(std::vector<color> new_color_set);

	

		/**
		 * \brief The list of colors to provide vertex oriented colors.
		 *
		 * The `colors` method will make use of the colors in this list.
		 * If the list is shorter than needed by the coloring algorithm (block
		 * uses six colors) the list is accessed cyclically.
		 *
		 * To serve its intended purpose, the list should have at least two colors.
		 *
		 * Example of setting:
		 *
		 * `sh->color_set_ = std::vector<cs4722::color>({cs4722::x11::goldenrod1, cs4722::x11::goldenrod3});`
		 *
		 *
		 */
		std::vector<color> color_set_;

		// where this shape starts in the various buffers
		/**
		 * \brief Location at which shape information begins in a buffer.
		 *
		 * Information for various shapes will be stored in the same buffer.
		 * This attribute specifies the index at which the information starts.
		 * Since the value is an index into a list of values (vec4 or vec2  or array of bytes) it works
		 * whatever the composition of the individual values.
		 *
		 */
		int buffer_start = 0;
		// how many components this shape uses in each buffer
		/**
		 * \brief Number of components used by a shape in a buffer
		 *
		 * This attribute specifies how many components are used in a buffer to store
		 * vertex data for this shape.
		 * This will usually be set to the value of `get_size()` during buffer creation.
		 *
		 *
		 */
		int buffer_size = 0;

		shape() 
			: drawing_mode(GL_TRIANGLES)
		{

		}

		/**
		 * \brief The OpenGL drawing mode to be used for this shape.
		 *
		 * The default drawing mode is `GL_TRIANGLES`.
		 * This can be overridden in the constructors for subclasses.
		 *
		 */
		GLenum drawing_mode;
		

	};


	/**
	 * \brief Represents a cube shape
	 *
	 *
	 * This shape represents a cube centered at the origin and length 1 on each side.
	 * Edges of the cube are parallel to the coordinate axes.
	 *
	 * Twelve triangles, two for each side, are used to cover the shape.
	 * This results in 36 vertices.
	 *
	 * Normal vectors, texture coordinates, colors, and tangent vectors may have different values
	 * for vertices in the same position, depending on the face to which the vertex belongs.
	 *
	 * Each face of the cube will be colored a solid color.
	 * Up to size colors from `color_set_` will be used.
	 *
	 */
	class block : public shape
	{
	public:

        /**
         * \brief Maximum value of texture coordinate on each face.
         *
         * The texture coordinates on each face will run from 0 to texture_scale.
         */
        double texture_scale = 1.0;

	    /**
	     * \brief Initializes the color set for this shape.
	     *
	     * The color set is initialized to the list blue, yellow, gray, white, orange, and black.
	     *
	     */
		block();


		/**
		 * \brief Returns 36
		 *
		 *
		 * @return  36, the number of vertices it takes to describe the triangles covering the cube.
		 */
		int get_size() override
		{
			return 36;
		}
		
		std::vector<glm::vec4>* positions() override;


		/**
		 * \brief Returns a list with a color for each vertex.
		 *  Each side of the block with a solid color.
		 *
		 * The color scheme for a block is to paint each side with a solid color.
		 * Up to six colors from 'color_set_' will be used.
		 *
		 *
		 */
		std::vector<cs4722::color>* colors() override;

		std::vector<glm::vec2>* texture_coordinates() override;

		std::vector<glm::vec4>* normals() override;

		std::vector<glm::vec4>* tangents() override;


		
	};


	/**
	 * \brief Represents a sphere of radius 1 centered at the origin.
	 *
	 * This shape approximates a sphere with bands of triangles around the sphere and two
	 * 'caps' at the poles.
	 * The number of bands between the caps can be specified as can the number of sides in  each band.
	 * In this counting, each side is a quadrilateral, so made up of two triangles.
	 *
	 *
	 */
	class sphere: public shape
	{
	public:


	    /**
	     * \brief Initialize the sphere shape using the number of bands and sides specified.
	     *
	     * @param bands  Must be at least 1.
	     * @param sides  Must be at least 3.
	     */
		sphere(int bands, int sides);
		

		/**
		 * \brief Initialize the sphere shape using 5 bands and 22 sides.
		 */
		sphere(): sphere(5, 22) {}

		/**
		 * \brief return the number of vertices used for this shape.
		 *
		 * @return 6 * sides * (bands + 1)
		 */
		int get_size() override
		{
			return 6 * sides_ * (bands_ + 1);
		}

		std::vector<glm::vec4>* positions() override;


		std::vector<cs4722::color>* colors() override;

		

		std::vector<glm::vec2>* texture_coordinates() override;

		std::vector<glm::vec4>* normals() override;


	private:

		int bands_, sides_;

	};

	/**
	 * \brief This shape approximates a cylinder as a prism and then triangulating the polygons making up
	 *  the prism.
	 *
	 *  Calling this a cylinder is not quite correct since the option exists to set the radii of the top and bottom
	 *  to different values.
	 *  So, this shape could be a cylinder or a cone or the frustum of a cone.
	 *  In any case, the radius of the middle of the shape, in the xz plane, is 1.
	 */
	class cylinder : public shape
	{
	public:


	    /**
	     * \brief Initialize the shape to have the specified ratio of the top radius to the bottom radius.
	     *  'sides' specifies the number of quadrilaterals used around the sides.
	     *
	     *  The actual radii of the top and bottom are adjusted so that the radius of the middle of the shape
	     *  is 1.
	     *
	     *  The top and the bottom of the cylinder are triangulated by triangles with a vertex at the center.
	     *  The number of these triangles at each end is equal to the number of sides.
	     *
	     * @param top_to_bottom_ratio  Must be non-negative
	     * @param sides Must be at least 3
	     */
		cylinder(float top_to_bottom_ratio, int sides);

		/**
		 * \brief  Intialize the shape to an actual cylinder, top radius equal to bottom, using 20
		 *  quadrilaterals around the side.
		 */
		cylinder() : cylinder(1.0, 20) {}


		/**
		 * \brief return the number of vertices used by this shape.
		 *
		 * @return 12 times the number of sides.
		 */
		int get_size() override
		{
			return 12 * sides_;
		}

		std::vector<glm::vec4>* positions() override;

		std::vector<cs4722::color>* colors() override;
		
		std::vector<glm::vec2>* texture_coordinates() override;
		
		std::vector<glm::vec4>* normals() override;


	private:
		float top_to_bottom_ratio_;
		int sides_;

	};


	/**
	 * \brief This 'shape' is the points that define the vertices of torus.
	 *
	 * \deprecated  This class was created as an intermediate step to the class torus.
	 *  It will ultimately be removed.
	 *
	 *
	 *
	 */
	class torus_points : public shape {
	public:

		torus_points(float inner_radius, int bands, int sides);

		int get_size();

		/*
		Get vertices
		*/
		std::vector<glm::vec4>* positions();



		std::vector<cs4722::color>* colors();

		std::vector<glm::vec4>* normals() override;

	private:
		float inner_radius_;
		int bands_, sides_;
	};


	/**
	 * \brief This shape approximates a torus.
	 *
	 * The main axis of the torus is aligned with the z-axis.
	 * The outer radius of the torus is 1, the inner radius can be specified in
	 * the constructor.
	 *
	 * Similar to the sphere, the torus is covered in quads numbered in bands (around the torus through
	 * the hole) and sides (around the outside of the torus).
	 *
	 */
	class torus : public shape {
	public:


	    /**
	     * \brief Initialize a torus using provided parameters.
	     *
	     *
	     *
	     * @param inner_radius should be non-negative and less than 1.
	     *  Some interesting effects result if these bounds are ignored.
	     * @param bands  Number of quads around the torus through the central hole.
	     *  Must be at least 3.
	     * @param sides Number of quads around the outside.
	     *  Must be at least 3.
	     */
		torus(float inner_radius, int bands, int sides);

		/**
		 * \brief Initialize the shape with default values.
		 *
		 * Initialize a torus with inner radius .5, 10 bands, and 20 sides.
		 */
		torus() : torus(.5, 10, 20){}

		int get_size() override;

		/*
		Get vertices
		*/
		std::vector<glm::vec4>* positions() override;



		std::vector<cs4722::color>* colors() override;

		std::vector<glm::vec2>* texture_coordinates() override;

		std::vector<glm::vec4>* normals() override;

		std::vector<glm::vec4>* tangents() override;
		
	private:
		float inner_radius_;
		int bands_, sides_;
	};



	/**
	 * \brief This 'shape' is a flat rectangle.
	 *
	 * For most purposes, this shape could be replaced by a block that is very thin in one dimension.
	 * However, this shape parameterizes the texture coordinates assigned to vertices, so it is used
	 * for illustrations of texturing.
	 *
	 * Texture coordinates will run from 0 to `texture_width_scale` in one dimension and
	 * 0 to `texture_height_scale` in the other.
	 *
	 * The rectangle is also divided up into a number of smaller rectangles.
	 */
	class rectangle : public shape {
		// a rectangle formed from sub-rectangles

	public: 

	    /**
	     * \brief Initialize the rectangle shape using the parameters provided and some default values.
	     *
	     * The rectangle is subdivided into smaller rectangles arranged in `width` columns and
	     * `height` rows.
	     *
	     * `texture_width_scale` and `texture_height_scale` are initialized to 1.0.
	     * These attributes can be changed as long as `texture_coordinates()` is not called before they
	     * are changed.
	     *
	     * @param width
	     * @param height
	     */
		rectangle(int width, int height) :
			width(width), height(height), texture_width_scale(1.0), texture_height_scale(1.0) {
			color_set_ = std::vector<color>({ x11::olive_drab1, x11::dark_goldenrod4, });
		}

		rectangle() : rectangle(4, 4) {}

		int get_size() override;

		
		std::vector<glm::vec4>* positions() override;

		std::vector<cs4722::color>* colors() override;

		std::vector<glm::vec2>* texture_coordinates() override;

		std::vector<glm::vec4>* normals() override;

		// double texture_width_scale() const
		// {
		// 	return texture_width_scale_;
		// }
		//
		// void set_texture_width_scale(double texture_width_scale)
		// {
		// 	texture_width_scale_ = texture_width_scale;
		// }
		//
		// double texture_height_scale() const
		// {
		// 	return texture_height_scale_;
		// }
		//
		// void set_texture_height_scale(double texture_height_scale)
		// {
		// 	texture_height_scale_ = texture_height_scale;
		// }

	// private:
		int width, height;
		double texture_width_scale;
		double texture_height_scale;;
	};

}


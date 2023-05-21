
/*
 * The change to the vertex shader from the last module is the introduction of the uniform
 * variable transform and using transform to change the position value from the buffer.
 */

#version 430 core

in layout(location = 1) vec4 bPosition;
in  layout(location = 2) vec4 bColor;

/*
 * The transform is set from the driver by code in the display function.
 */
uniform layout(location = 1) mat4 transform;

out vec4 vColor;

void
main()
{
    vColor = bColor;
    /*
     * The * operator implements matrix multiplication in GLSL.
     * The output position is the transformed position.
     *
     * Note that this does not change bPosition.
     */
    gl_Position = transform * bPosition;
//    gl_Position = bPosition;
}

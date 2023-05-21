
#version 430 core

in layout(location = 1) vec4 bPosition;
in layout(location = 2) vec2 bTextureCoord;
in layout(location = 3) vec4 bNormal;

uniform layout(location = 1) mat4 vp_transform;
uniform layout(location = 2) mat4 n_transform;
uniform layout(location = 3) mat4 m_transform;


out vec2 vTextureCoord;
out vec4 wPosition;     // in world coordinates
out vec4 oPosition;   // in object coordinates
out vec4 wNormal;       // in world coordinates



void
main()
{
    /*
     * The oPosition is used by the skybox to determine samples from the cube texture
     */
    oPosition = bPosition;
    /*
     * wPosition is the position in world coordinates.
     * This is used to compute the reflection vector
     */
    wPosition = m_transform * bPosition;
    /*
     * The normal vector in world coordinates, also used to compute the reflection
     */
    wNormal = n_transform * bNormal;
    // send over the texture coordinates to be used by ordinary objectgs
    vTextureCoord = bTextureCoord;
    /*
     * Effectively projection_transform * view_transform * model_transform * bPosition
     */
    gl_Position = vp_transform * wPosition;
}

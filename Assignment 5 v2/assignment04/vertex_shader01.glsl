
#version 430 core

in layout(location = 1) vec4 bPosition;
in layout(location = 2) vec2 bTextureCoord;
in layout(location = 3) vec4 bColor;
in layout(location = 4) vec4 bNormal;

uniform layout(location = 10) mat4 transform;
uniform layout(location = 6) mat4 vp_transform;
uniform layout(location = 7) mat4 n_transform;
uniform layout(location = 8) mat4 m_transform;
uniform layout(location = 9) vec3 camera_position;

out vec2 vTextureCoord;
out vec4 vColor;
out vec4 wNormal;       // in world coordinates
out vec4 oPosition;   // in object coordinates
out vec4 wPosition;     // in world coordinates

void
main()
{
    vTextureCoord = bTextureCoord;
    oPosition = bPosition;
    gl_Position = transform * bPosition;
    vColor = bColor;
    wNormal = n_transform * bNormal;
    wPosition = m_transform * bPosition;
}

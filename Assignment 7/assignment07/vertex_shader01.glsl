
#version 430 core

in layout(location = 1) vec4 bPosition;
in layout(location = 2) vec4 bNormal;

uniform layout(location = 1) mat4 m_transform;
uniform layout(location = 2) mat4 vp_transform;
uniform layout(location = 3) mat4 normal_transform;


out vec4 wNormal;
out vec4 wPosition;

void
main()
{
    wNormal = normal_transform * bNormal;
    wPosition = m_transform * bPosition;
    gl_Position =  vp_transform * wPosition;
}

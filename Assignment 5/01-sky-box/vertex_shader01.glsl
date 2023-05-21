
#version 430 core

in layout(location = 1) vec4 bPosition;
in layout(location = 2) vec2 bTextureCoord;

uniform layout(location = 1) mat4 transform;

out vec2 vTextureCoord;
// send the object coordinates to the fragment shader
out vec4 vPosition;

void
main()
{
    vPosition = bPosition;
    vTextureCoord = bTextureCoord;
    gl_Position = transform * bPosition;
}

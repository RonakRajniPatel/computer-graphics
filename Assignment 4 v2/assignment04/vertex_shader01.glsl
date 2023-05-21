
#version 430 core

in layout(location = 1) vec4 bPosition;
in layout(location = 2) vec2 bTextureCoord;
in layout(location = 3) vec4 bColor;

uniform layout(location = 1) mat4 transform;

out vec2 vTextureCoord;
out vec4 vColor;

void
main()
{
    vTextureCoord = bTextureCoord;
    gl_Position = transform * bPosition;
    vColor = bColor;
}

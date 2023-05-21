
#version 430 core

in layout(location = 1) vec4 bPosition;
in layout(location = 2) vec4 bColor;
in layout(location = 5) vec3 bNormal;


uniform layout(location = 1) mat4 transform;

out vec4 vColor;
out vec2 vTxtCrd;

void
main()
{
    vColor = bColor;
    gl_Position = transform * bPosition;
}


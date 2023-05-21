#version 430 core

in layout(location=1) vec4 vPosition;

void main()
{
    gl_Position = vPosition;
}

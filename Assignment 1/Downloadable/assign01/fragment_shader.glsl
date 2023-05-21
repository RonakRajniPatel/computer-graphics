#version 430 core

out vec4 fColor;

uniform layout(location = 1) vec4 color;

void main()
{
    fColor = color;
}

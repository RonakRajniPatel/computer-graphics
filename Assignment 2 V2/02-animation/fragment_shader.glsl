#version 430 core

out vec4 fColor;

in vec4 vColor;

void main()
{
    fColor = vColor;
//    fColor = vec4(0,1,0,1);
}

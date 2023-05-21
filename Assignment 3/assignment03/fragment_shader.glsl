#version 430 core

out vec4 fColor;
in vec4 vColor;
in vec2 vTxtCrd;
in vec4 frag_color;
uniform layout(location = 6) float specular;
uniform layout(location = 2)vec4 uColor = vec4(1,1,1,1);
void main()
{
    fColor = vColor;
    fColor = uColor * vColor;
}
#version 450 core

out vec4 fColor;

in vec2 vTextureCoord;
in vec4 vPosition;

uniform layout(location = 2) sampler2D  sampler;
uniform layout(location = 5) samplerCube  samplerC;
uniform layout(location = 3) int surface_effect;
uniform layout(location = 4) vec4 diffuse_color;

void main()
{
    if(surface_effect == 0) {
        fColor = diffuse_color * texture(sampler, vTextureCoord);
    } else if(surface_effect == 1) {
        fColor = texture(sampler, vTextureCoord);
    } else if(surface_effect == 2) {
        fColor = diffuse_color;
    } else if(surface_effect == 32) {
        fColor = texture(samplerC, vPosition.xyz);
    } else {
        fColor = vec4(0,1,0,1);
    }
}

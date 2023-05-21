#version 430 core

out vec4 fColor;

in vec2 vTextureCoord;
in vec4 vColor;
in vec4 wNormal;
in vec4 oPosition;
in vec4 wPosition;

uniform layout(location = 2) sampler2D  sampler;
uniform layout(location = 3) int surface_effect;
uniform layout(location = 4) vec4 diffuse_color;
uniform layout(location = 5) vec3 camera_position;
uniform layout(location = 6) samplerCube  samplerC;

void main()
{
    //change 5
    if (surface_effect == 0) {
        fColor = vColor;
    }
    else if(surface_effect == 1) {
        fColor = diffuse_color;
    }
    else if(surface_effect == 2) {
        fColor = texture(sampler, vTextureCoord);
    }
    else if(surface_effect == 3) {
        fColor = vColor * texture(sampler, vTextureCoord);
    }
    else if(surface_effect == 4) {
        fColor = vColor * diffuse_color;
    }
    else if(surface_effect == 5) { //skybox else
        fColor = texture(samplerC, oPosition.xyz);
    }
    else if(surface_effect == 6) { //reflected vector
        vec3 rfl = reflect(wPosition.xyz-camera_position, normalize(wNormal.xyz));
        fColor = texture(samplerC, rfl) * vec4(.9);
    }
    else {
        fColor = vec4(0, 0, 1, 1);
    }
    //end
}

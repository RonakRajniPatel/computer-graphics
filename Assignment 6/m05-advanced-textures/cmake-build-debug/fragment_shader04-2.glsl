#version 450 core

out vec4 fColor;


in vec2 vTextureCoord;
in vec4 wPosition;
in vec4 oPosition;
in vec4 wNormal;


uniform layout(location = 4) sampler2D  sampler2;
uniform layout(location = 5) samplerCube  samplerC;
uniform layout(location = 6) int surface_effect;
uniform layout(location = 7) vec3 camera_position;
uniform layout(location = 8) vec4 diffuse_color;

void main()
{
    if(surface_effect == 0) {
        fColor = diffuse_color * texture(sampler2, vTextureCoord);
    } else if(surface_effect == 1) {
        fColor = texture(sampler2, vTextureCoord);
    } else if(surface_effect == 2) {
        fColor = diffuse_color;
    } else if(surface_effect == 33) {
        // reflection
        vec3 rfl = reflect(wPosition.xyz-camera_position, normalize(wNormal.xyz));
        /*
         * The sample color is being dimmed slightly by multiplying all components by .9.
         * This helps the reflecting object stand out a bit from the background.
         */
        fColor = texture(samplerC, rfl) * vec4(.9);
    } else {
        fColor = vec4(0,1,0,1);
    }
}

#version 430 core

out vec4 fColor;

in vec2 vTextureCoord;
in vec4 wPosition;
in vec4 oPosition;
in vec4 wNormal;


uniform layout(location = 4) sampler2D  sampler2;
uniform layout(location = 5) samplerCube  samplerC;
uniform layout(location = 6) int surface_effect;
uniform layout(location = 7) vec3 camera_position;

void main()
{
    if(surface_effect == 0) {
        // regular objects, 2D texture
        fColor = texture(sampler2, vTextureCoord);
    } else if(surface_effect == 1) {
        // skybox, uses the cube texture directly
        fColor = texture(samplerC, oPosition.xyz);

//        float g = 0.2126*fColor.r + 0.7152*fColor.g + 0.0722*fColor.b;
//        fColor = vec4(g, g, g, 1.0);
    } else if(surface_effect == 2) {
        /*
         * The vector from the camera to the fragment is reflected in the surface and this reflected vector
         * is used to get a sample from the cube texture.
         *
         * wNormal is an interpolation of the normal vectors at the three corners of the triangle.
         *
         */
        vec3 rfl = reflect(wPosition.xyz-camera_position, normalize(wNormal.xyz));
        /*
         * The sample color is being dimmed slightly by multiplying all components by .9.
         * This helps the reflecting object stand out a bit from the background.
         */
        fColor = texture(samplerC, rfl) * vec4(.9);
    } else {
        // shouldn't happen, but use yellow if the surface effect didn't match previous clauses.
        fColor = vec4(1, 1, 0, 1);
    }

//    fColor = vec4(fColor.r, 0,0,1);
}



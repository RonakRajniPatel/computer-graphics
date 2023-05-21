#version 430 core

out vec4 fColor;

//  use vTextureCoord for 2D textures (surface effect 0)
in vec2 vTextureCoord;
// use the coordinates in the shape coordinate frame for the cube texture
// The vertex positions are interpolated, so this is the position of the current fragment
//      in the original shape coordinates.
in vec4 vPosition;
in vec4 vColor;

uniform layout(location = 2) sampler2D  sampler2;
uniform layout(location = 3) samplerCube  samplerC;
uniform layout(location = 4) int surface_effect;
uniform layout(location = 5) vec4 diffuse_color;

void main()
{
    /*
     * Surface_effect is used to distinguish between the skybox and the scene.
     */
    if(surface_effect == 0) {
        fColor = texture(sampler2, vTextureCoord);
    }
    else if(surface_effect == 1) {
        fColor = texture(samplerC, vPosition.xyz);
    }
    else if(surface_effect == 2) {
        fColor = diffuse_color;
    }
    else if(surface_effect == 3) {
        fColor = texture(sampler2, vTextureCoord);
    }
    else if(surface_effect == 4) {
        fColor = diffuse_color * texture(sampler2, vTextureCoord);
    }
    else if(surface_effect == 5) {
        fColor = vColor;
    }
    else if(surface_effect == 6) {
        fColor = vColor * diffuse_color;
    }
    else {
        /*
            A line is drawn from the center of the artifact to the specified position.
            this line is extended out to the cube map and where it intersects, that is the sample.
        */
        fColor = texture(samplerC, vPosition.xyz);
    }
}

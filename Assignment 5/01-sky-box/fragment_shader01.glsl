#version 430 core

out vec4 fColor;

//  use vTextureCoord for 2D textures (surface effect 0)
in vec2 vTextureCoord;
// use the coordinates in the shape coordinate frame for the cube texture
// The vertex positions are interpolated, so this is the position of the current fragment
//      in the original shape coordinates.
in vec4 vPosition;

uniform layout(location = 2) sampler2D  sampler2;
uniform layout(location = 3) samplerCube  samplerC;
uniform layout(location = 4) int surface_effect;

void main()
{
    /*
     * Surface_effect is used to distinguish between the skybox and the scene.
     */
    if(surface_effect == 0) {
        fColor = texture(sampler2, vTextureCoord);
    } else {
        /*
            A line is drawn from the center of the artifact to the specified position.
            this line is extended out to the cube map and where it intersects, that is the sample.
        */
        fColor = texture(samplerC, vPosition.xyz);
    }
}

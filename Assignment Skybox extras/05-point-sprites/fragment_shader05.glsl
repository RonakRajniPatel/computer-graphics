#version 430 core


out vec4 fColor;

in vec4 vColor;
uniform layout(location = 2) sampler2D  sampler;
uniform layout(location = 3) int surface_effect;

void main()
{
    if(surface_effect == 1) {
        // gl_PointCoord provides the coordinates of this fragment inside the point
        //     The coordinates are in the range from 0 to 1.
        fColor = texture(sampler, gl_PointCoord);
    } else {
        fColor = vColor;
    }
}

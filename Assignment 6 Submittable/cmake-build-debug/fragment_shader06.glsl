#version 430 core

/*
 * Decorate each point with a circle colored with a gradient from dark red in the center
 * to very light magenta on the perimeter.
 * The center is also translucent while the perimeter is opaque.
 */


out vec4 fColor;



void main()
{

    // center color
    // note that alpha is .5
    // since blending is being used, this will allow objects behind this one to show through
    const vec4 color1 = vec4(0.6, 0.0, 0.0, .5);
    // perimeter color
    const vec4 color2 = vec4(0.9, 0.7, 1.0, 1.0);

    // Determine the distance of the current fragment from the center of the square
    //      Will use the square of the distance since that's quicker to computer and
    //      equivalent to use.
    // The center of the square is (0.5, 0.5)
    vec2 temp = gl_PointCoord - vec2(0.5, 0.5);
    float f = dot(temp, temp);  // square of the distance of the fragment from the center of the point


    // The largest circle that will fit in the square is radius .5 (diameter 1)
    // The square of the radius is .25.
    float t = 0.25;


    if(f > t) {
        // if f > t then the fragment is outside the circle.
        // don't even draw the fragment
        // this fragment isn't blended or compared with any other fragment, it's completely
        //      skipped
        discard;
//        fColor = vec4(1,1,0,1);
    } else {
        // gradient is computed using the mix function
        // smoothstep computes  a smoothly varying value between 0 and 1 based on
        //      the parameters.
        fColor = mix(color1, color2, smoothstep(0.1, t, f));
    }

//    fColor = vec4(gl_PointCoord, 0, 1);  // debugging, see if gl_PointCoord is working
}

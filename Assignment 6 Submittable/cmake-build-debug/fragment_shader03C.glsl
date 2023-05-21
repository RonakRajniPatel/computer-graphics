#version 430 core

/*
 *  As distributed, the fragment shader just presents a sky box and a couple of objects.
 *
 *  Read through the code below and follow the instructions to uncomment different statements to see the effect.
 *  For example, if you uncomment the line
 *        //  fColor = vec4(base_color.r, 0.0, 0.0, 1.0);
 *  you will see the result of a red filter.
 */


out vec4 fColor;

in vec2 vTextureCoord;
in vec4 vPosition;

uniform layout(location = 2) sampler2D  sampler2;
uniform layout(location = 3) samplerCube  samplerC;
uniform layout(location = 4) int surface_effect;

void main()
{
    vec4 base_color;
    if(surface_effect == 0) {
        base_color = texture(sampler2, vTextureCoord);
    } else {
        base_color = texture(samplerC, vPosition.xyz);
    }



    // unchanged
    fColor = base_color;


    /*
     *  Uncomment the line after this comment to see a red filter in action.
     *      This filter throws away the green and blue components of the color, just retaining the red.
     */
//    fColor = vec4(base_color.r, 0.0, 0.0, 1.0);

    // yellow filter
    // fColor = vec4(base_color.r, base_color.g, 0.0, 1.0);

    /*
     * We can get effects not possible (at least easily) with camera filters.
     * Uncomment the next line to see what looks like the result of a green filter.
     * However, the blue component is being used to set the visible green.
     */
//     fColor = vec4(0.0, base_color.b, 0.0, 1.0);

    /*
     * There are various approaches to creating a gray scale image.
     * One very good approach is to use luminance, a value which represents the overall brightness of
     * a color.
     * The idea is to take a weighted average of the R, G, and B components of the color and then
     *      use that average value for all three components of the fragment color.
     *      This results in a gray color with the same perceived brightness.
     *
     *  Formula from     https://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color
     *      luminance = 0.2126 * R + 0.7152 * G + 0.0722 * B
     *  You will notice that green gets a great deal of weight.  This is because green is perceived as brighter by
     *      our eyes than red or blue of the same strength.
     *  On the other hand, blue is not perceived as very bright, so the blue component receives a very low weight.
     *  Do not uncomment out the next line since that value is used in filters later in the code.
     */
    float g = 0.2126*base_color.r + 0.7152*base_color.g + 0.0722*base_color.b;
    // uncomment this next line to see a luminance based image
//     fColor = vec4(g, g, g, 1.0);



//    if(surface_effect == 1) {
//        fColor = vec4(g, g, g, 1.0);
//    }

    /*
     * Coded colors do not try to preserve the actual original color in the final image but will replace
     * that with a color intended to identify specific characteristics of that color.
     *
     * The following example color low luminance fragments with blue, medium luminance fragments with red,
     *      and high luminance fragments with green.
     *
     * Because the code for this image is more than one line,
     * To see the coded luminance example work, remove the space between the * and / at the end
     *      of the following comment.  Put the space back in to remove the example
     */

    /*  coded luminance * /
    if(g < 0.3)
        fColor = vec4(0,0,1,1);
    else if(g < 0.6)
        fColor = vec4(1,0,0,1);
    else
        fColor = vec4(0,1,0,1);
    /* */

    /*
     * The following filter mixes blue and green in proportion to the luminance:
     *      more green for brighter areas, more blue for darker areas
     *  Just uncomment the following line to see how that looks.
     */
//     fColor = mix(vec4(0,0,1,1), vec4(0,1,0,1), g);


    /*
     * A gray scale filter can be constructed in many ways.
     * A straight average of the color components can be used.  This won't have
     * the correct perceived brightness, but it will be close.
     *
     * To see the simpler luminance example work, remove the space between the * and / at the end
     * of the following comment.  Put the space back in to remove the example
     */
    /*  Simpler version of luminance * /
    float g2 = (base_color.r + base_color.g + base_color.b) / 3.0;
    fColor = vec4(g2, g2, g2, 1.0);
    /* */


    /*
     * The following gray scale image just uses the red component.
     * Uncomment the line to see the effect.
     */
//     fColor = vec4(base_color.r, base_color.r, base_color.r, 1.0);

    /*
     * Cameras using film usually record colors in negative, the color intensities are
     *  reversed.
     *  Reversing color values can be accomplished by subtracting from 1.
     *  Uncomment the next line to see.
     */
//      fColor = vec4(1.0 - base_color.r, 1.0 - base_color.g, 1.0 - base_color.b, 1.0);


// for use in another example
//    float g = 0.2126*fColor.r + 0.7152*fColor.g + 0.0722*fColor.b;
//    fColor = vec4(g, g, g, 1.0);

}

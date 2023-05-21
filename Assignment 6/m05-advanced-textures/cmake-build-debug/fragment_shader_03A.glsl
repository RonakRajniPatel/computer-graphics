#version 430 core

/**
 * The examples in this fragment shader determine the color of a fragment by examining nearby pixels.
 *
 * These calculations cannot be done reliably without first rendering the scene in its entirety.
 * If we were to use the values in the framebuffer while the scene was being rendered we would inevitably
 *      end up using partially determined values or even the background.
 *      The problem is that some fragments will be computed before others and that means that some
 *      neighboring fragments might not have been computed yet.
 *
 * As described in the driver comments (image-processing.cpp), the key is to render the scene to a texture and
 *      then use the texture as the data source for the algorithms here.
 */

/**
 *  Two types of algorithms are illustrated here.
 *  These roughly correspond to integration and differentiation in calculus.
 *
 *  The first two examples illustrate smoothing data by using the average color of several pixels close to the
 *  fragment being colored.
 *
 *  The third example illustrates finding abrupt changes in the image.  These can be used to find edges
 *  in an image.
 *
 *
 */
 /**
  *     All three examples are disabled to start with.
  *     Please read the comments below and examine the effect of each algorithm.
  *
  */


out vec4 fColor;

in vec2 vTextureCoord;

uniform layout(location = 8) sampler2D  sampler;
uniform layout(location = 9) ivec2 fb_size;

/**
* Smoothing is a matter of averaging together the values of several pixels.
* We will just illustrate equal weighting, but better results might be achieved with
*      weighting some pixels more than others.
*
* Interestingly, the value of the texture at the current fragment is not used, just four of the
* neighbors.
*
* To see the smoothing example work, remove the space between the * and / at the end
*    of the following comment.  Put the space back in to remove the example
*
*  I have a hard time seeing the change here.  The effect in the next version is easier to see.
*/
void simple_smoothing(vec2 delta) {

    // tc is the texture coordinates, adding (delta, 0) results in the coordinates of the pixel to the right
    //      of this fragment.
    vec4 s1 = texture(sampler, vTextureCoord + vec2(delta.x,0));
    //  above this gragment
    vec4 s2 = texture(sampler, vTextureCoord + vec2(0,delta.y));
    // below this fragment
    vec4 s3 = texture(sampler, vTextureCoord + vec2(0,-delta.y));
    // to the left of this fragment
    vec4 s4 = texture(sampler, vTextureCoord + vec2(-delta.x,0));
    // take the average
    // note how we can add vec4's and do scalar division
    fColor = (s1 + s2 + s3 + s4) / 4.0;

}


/**
 *  This example uses a larger number of samples.
 *  The number of samples is (2*lim+1)*(2*lim+1).
 *  With lim - 5 that is 121 samples.
 *
 *  The smoothing effect becomes much more pronounced as lim is increased, but rendering slows down.
 *
 *  To see this example, remove the space in * / at the end of the line.
 *  To disable this example again, add the space back in.
 */
void higher_order_smoothing(vec2 delta) {
    vec4 s = vec4(0,0,0,0);
    const int lim = 5;
    for(int i = -lim; i <= lim; i++ ) {
        for(int j = -lim; j <= lim; j++) {
            s += texture(sampler, vTextureCoord + vec2(i*delta.x, j*delta.y));
        }
    }
    fColor = s / ((2*lim+1)*(2*lim+1));
}


/**
 *  The derivative is a difficult operation to approximate using sample data.
 *  It is very easy to end up with noise rather than useful results.
 *  We will look at just one method of approximating the rate of change of colors, the Sobel derivative.
 *
 *  The easiest way to approximate a derivative is to simply subtract neighboring values.
 *  However, this can often provide results that are difficult to interpret.
 *  It is better to use more neighboring values.
 *  The Sobel derivative is one such scheme.
 *
 *  Similarly to smoothing, we use a weighted average of samples.
 *  However, some of the samples have negative weights, so we are computing changes here.
 *
 *  When you view the image you will note that the processed image tends to highlight edges between
 *      contrasting areas.
 *
 *  One more point: there are actually two derivatives at any point: one is the change in the x direction;
 *      the other is the change in the y direction.
 *      These are 'partial derivatives' in mathematical parlance.
 *
 *
 *
 * to see the sobel derivative example work, remove the space between the * and / at the end
 * of the following comment.  Put the space back in to remove the example
 */
void sobel_derivative(vec2 delta) {
   // first compute the derivative in the x direction
    // three samples to the left of the fragment
    vec4 u1 = texture(sampler, vTextureCoord + vec2(-delta.x, -delta.y));
    vec4 u2 = texture(sampler, vTextureCoord + vec2(-delta.x, 0));
    vec4 u3 = texture(sampler, vTextureCoord + vec2(-delta.x, delta.y));
    // three samples to the right of the fragment
    vec4 t1 = texture(sampler, vTextureCoord + vec2(delta.x, -delta.y));
    vec4 t2 = texture(sampler, vTextureCoord + vec2(delta.x, 0));
    vec4 t3 = texture(sampler, vTextureCoord + vec2(delta.x, delta.y));
    //  weighted average and difference
    vec4 sobelX = ((u1 + 2*u2 + u3) - (t1 + 2*t2 + t3)) / 8.0;
    // all we really use here is the length of the vector, a measure of the rate of change
    float dX = length(sobelX.xyz);

    u1 = texture(sampler, vTextureCoord + vec2(-delta.x, -delta.y));
    u2 = texture(sampler, vTextureCoord + vec2(0, -delta.y));
    u3 = texture(sampler, vTextureCoord + vec2(delta.x, -delta.y));
    t1 = texture(sampler, vTextureCoord + vec2(-delta.x, delta.y));
    t2 = texture(sampler, vTextureCoord + vec2(0, delta.x));
    t3 = texture(sampler, vTextureCoord + vec2(delta.x, delta.y));
    vec4 sobelY = ((u1 + 2*u2 + u3) - (t1 + 2*t2 + t3)) / 8.0;
    // again the length of the vector, this is a measure of the rate of change
    float dY = length(sobelY.xyz);

    // Harmonic  average of the two rates of change to get an overall estimate of the
    //      rate of change in all directions at this fragment.
    float d = sqrt(dX*dX + dY*dY);

    // display as a gray level
    //      Darker shades of gray are low rates of change
    //      Lighter shades of gray are higher rates of change
    fColor = vec4(d, d, d, 1);
}



void main()
{

    // default
    fColor = texture(sampler, vTextureCoord);


    /**
     * Even though the source texture is composed of discrete pixels, we must use floating point
     * numbers in the range from 0 to 1 for the coordinates used to access those pixels.
     *
     * Since we want to access pixels near to the target fragment, we need to know the difference between
     *      coordinates of adjacent pixels.
     *      'delta' is that value.
     *
     *
     */
    vec2 delta = 1.0 / fb_size;


    /*
        Uncomment one of the three lines below to see the transformation effect.
        Comment all three lines to see the base image.
    */
//    simple_smoothing(delta);
//    higher_order_smoothing(delta);
//    sobel_derivative(delta);

   
}


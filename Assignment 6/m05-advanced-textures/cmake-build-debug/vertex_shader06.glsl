#version 430 core

in layout(location = 1) vec4 bPosition;


uniform layout(location = 1) mat4 transform;


void
main()
{
//    gl_PointSize = 400.0;
    vec4 pos = transform * bPosition;
    //gl_PointSize =(3.0 -  pos.z/pos.w) * 64;
    gl_Position = pos;
}

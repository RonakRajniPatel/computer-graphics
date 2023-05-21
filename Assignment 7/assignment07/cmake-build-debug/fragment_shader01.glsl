#version 430 core

out vec4 fColor;

in vec4 wNormal;
in vec4 wPosition;

// light
uniform layout(location = 4) vec4 ambient_light;
uniform layout(location = 5) vec4 specular_light;
uniform layout(location = 6) vec4 diffuse_light;
uniform layout(location = 7) vec4 light_direction; // position of the light
uniform layout(location = 8) vec4 camera_position;

// material
uniform layout(location = 9) vec4 ambient_color;
uniform layout(location = 10) vec4 specular_color;
uniform layout(location = 11) vec4 diffuse_color;
uniform layout(location = 12) float specular_shininess;     // exponent for sharpening highlights
uniform layout(location = 13) float specular_strength;      // extra factor to adjust shininess


void main()
{
    fColor = vec4(normalize(wNormal.xyz)/2 + .5, 1.0);

    vec3 vnn = normalize(wNormal.xyz);

    float diffuse_factor = max(0.0, dot(vnn, -normalize(light_direction.xyz)));


    vec3 half_vector = normalize(normalize(-light_direction.xyz) - normalize(wPosition.xyz-camera_position.xyz));
    float specular_factor = max(0.0, dot(vnn, half_vector));

    if (diffuse_factor == 0.0)
    specular_factor = 0.0;
    else
    specular_factor = pow(specular_factor, specular_shininess) * specular_strength;  // sharpen the highlight


    vec4 ambient_component = ambient_color * ambient_light;
    vec4 diffuse_component = diffuse_factor * diffuse_color * diffuse_light;
    vec4 specular_component = specular_factor * specular_color * specular_light;


    vec4 total = ambient_component + diffuse_component + specular_component;


    fColor = vec4(total.rgb, 1.0);




}

#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position_roof;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color = object_color;

        
    
    out_color = vec4(color,0.5f);
}

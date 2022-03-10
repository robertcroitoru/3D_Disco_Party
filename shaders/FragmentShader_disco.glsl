#version 330

// Input
in vec2 texcoord;
in int mode_globe;

// Uniform properties
uniform sampler2D texture_1;
uniform int mode;

//uniform float Time;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    
   if (mode % 3 == 2)
   {    
        vec4 color1 = texture (texture_1, texcoord);
        out_color = color1;
   
        if (out_color.a < 0.5f)
            discard;
    }
    else
        out_color = vec4(0,0,0,1);
    // TODO(student): Calculate the out_color using the texture2D() function.
    //out_color = vec4(color);

}

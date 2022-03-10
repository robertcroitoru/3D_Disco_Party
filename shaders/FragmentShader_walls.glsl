#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_direction_spot;
uniform vec3 light_position[8];
uniform vec3 disco_light_pos;

uniform vec3 color_spot[4];
uniform vec3 spot_position[4];

uniform vec3 color_box[8];

uniform sampler2D texture_1;

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float Time;
uniform int mode;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform float cuttOffAngle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 color;
        
        vec3 L[9];
        for (int i = 0; i < 8; i++)
            L[i] = normalize(light_position[i] - world_position);
        L[8] = normalize(disco_light_pos - world_position);

        vec3 L_spot[4];
        for (int i = 0; i < 4; i++)
            L_spot[i] = normalize(spot_position[i] - world_position);

        vec3 V = normalize(eye_position - world_position);

        vec3 H[9];
        for (int i = 0; i < 8; i++)
            H[i] = normalize(normalize(L[i] + V));
        H[8] = normalize(normalize(L[8] + V));

        vec3 H_spot[4];
        for (int i = 0; i < 4; i++)
            H[i] = normalize(normalize(L_spot[i] + V));

        vec3 R[9];
        for (int i = 0; i < 8; i++)
            R[i] = reflect (-L[i], world_normal);
        R[8] = reflect (-L[8], world_normal);

        vec3 R_spot[4];
        for (int i = 0; i < 4; i++)
            R_spot[i] = reflect (-L_spot[i], world_normal);

         // TODO(student): Define ambient, diffuse and specular light components
        float ambient_light = 0.1 * material_kd;

        float diffuse_light[9];
        for (int i = 0; i < 8; i++)
            diffuse_light[i] =  material_kd * 1 * max(dot(world_normal,L[i]), 0);
        diffuse_light[8] = material_kd * 1 * max(dot(world_normal,L[8]), 0);

        float diffuse_light_spot[4];
        for (int i = 0; i < 4; i++)
            diffuse_light_spot[i] =  material_kd * 1 * max(dot(world_normal,L_spot[i]), 0);
   
        float specular_light[9];
        for (int i = 0; i < 8; i++)
            specular_light[i] = 0;
        specular_light[8] = 0;

        float specular_light_spot[4];
        for (int i = 0; i < 4; i++)
            specular_light_spot[i] = 0;

        // It's important to distinguish between "reflection model" and
        // "shading method". In this shader, we are experimenting with the Phong
        // (1975) and Blinn-Phong (1977) reflection models, and we are using the
        // Phong (1975) shading method. Don't mix them up!
        for (int i = 0; i < 8; i++)
        {
            if (diffuse_light[i] > 0)
            {
                specular_light[i] = material_ks * pow(max(dot(V, R[i]), 0), material_shininess);
            }
        }
        if (diffuse_light[8] > 0)
        {
            specular_light[8] = material_ks * pow(max(dot(V, R[8]), 0), material_shininess);
        }

        for (int i = 0; i < 4; i++)
        {
            if (diffuse_light_spot[i] > 0)
            {
                specular_light_spot[i] = material_ks * pow(max(dot(V, R_spot[i]), 0), material_shininess);
            }
        }

            // TODO(student): If (and only if) the light is a spotlight, we need to do
            // some additional things.
        {
        
       
            float dist[9];
            for (int i = 0; i < 8; i++)
                dist[i]= distance(light_position[i], eye_position);
            dist[8] =  distance(disco_light_pos, eye_position);

            float attenuation_factor[9];
            for (int i = 0; i < 8; i++)
                attenuation_factor[i] = 1/1 / (1 + 0.01 * dist[i] + 0.05 * pow(dist[i],2)); // 1/(kc + kl*d+ kq*d^2)
            attenuation_factor[8] = 1/1 / (1 + 0.01 * dist[8] + 0.05 * pow(dist[8],2));
            //float attenuation_factor = 1/1 / (1 + 0.01 * dist + 0.05 * pow(dist,2)); // 1/(kc + kl*d+ kq*d^2);
            // TODO(student): Compute the total light. You can just add the components
            // together, but if you're feeling extra fancy, you can add individual
            // colors to the light components. To do that, pick some vec3 colors that
            // you like, and multiply them with the respective light components.
     if (mode % 3 == 0)
     {
            color = vec3(object_color * 0.3f);
            for (int i = 0; i < 8; i++)
                color += 0.01*color_box[i] * (ambient_light + attenuation_factor[i] *(8*diffuse_light[i] + 100*specular_light[i]));
            // TODO(student): Write pixel out color

            color += 0.1*object_color * (ambient_light + attenuation_factor[8]* (20*diffuse_light[8] + 5*specular_light[8]));
            out_color = vec4(color,1);
    }
    else if (mode % 3 == 1)

    {    
        color = vec3(object_color * 0.3f);
        float cut_off = radians(cuttOffAngle);
        float spot_light[4];
        for (int i = 0; i < 4; i++)
            spot_light[i] = dot(-L_spot[i], light_direction_spot);
        float spot_light_limit = cos(cut_off);

        float linear_att[4];
        float light_att_factor[4];
        for (int i = 0; i < 4; i++)
        {
           if (spot_light[i] > cos(cut_off))
           {
                linear_att[i] = (spot_light[i] - spot_light_limit) / (1.0f - spot_light_limit);
                light_att_factor[i] = pow(linear_att[i], 2);
                color += color_spot[i]* (ambient_light + light_att_factor[i] * (5*diffuse_light_spot[i] + 5*specular_light_spot[i]));
           }
           else
                color += 0.1*ambient_light;

        }
         out_color = vec4(color,1);
      }
      else 
      {
        color = vec3(1);
        vec3 light_dir = world_position - disco_light_pos;
        vec2 texcooord;
        texcooord.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z);
        texcooord.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));

        if (Time > 0)
	    {
		    texcooord = vec2(texcooord.x - Time / 10.f, texcooord.y);
	    }
        color += 0.1* (ambient_light + attenuation_factor[8]* (50*diffuse_light[8] + 10*specular_light[8]));
        vec4 color1 = texture (texture_1, texcooord)/4 * vec4(color,0);
        out_color = color1;
      }
        
    }
    
}

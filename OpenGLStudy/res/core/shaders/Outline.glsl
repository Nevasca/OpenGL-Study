#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceModelMatrix;    

uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoord;

void main()
{
    gl_Position = u_Proj * u_View * a_InstanceModelMatrix * a_Position; // Instancing approach

    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

// Material
uniform sampler2D u_Diffuse;
uniform vec4 u_OutlineColor;

void main()
{
    float alpha = texture(u_Diffuse, v_TexCoord).a;

    // TODO: have alpha cutout enabled and threshold coming from uniforms
    if(alpha < 0.1f)
    {
        discard;
    }
      
    o_Color = u_OutlineColor;
}
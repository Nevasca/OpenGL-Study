#shader vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;  

out vec2 v_UV;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(a_Position, 0.0f, 1.f);
    v_UV = a_UV;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_UV;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;

void main()
{          
    // Text texture data is stored in just its red component, we sample the .r as the alpha value
    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(u_Text, v_UV).r);
    o_Color = vec4(u_TextColor, 1.f) * sampled;
}
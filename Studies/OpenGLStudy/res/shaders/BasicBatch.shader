#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TexIndex;   

out vec4 v_Color;
out vec2 v_UV;
out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_Position;

    v_Color = a_Color;
    v_UV = a_UV;
    v_TexIndex = a_TexIndex;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_UV;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{          
    int index = int(v_TexIndex);
    o_Color = texture(u_Textures[index], v_UV);
}
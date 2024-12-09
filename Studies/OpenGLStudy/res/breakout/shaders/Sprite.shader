#shader vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;  

out vec2 v_UV;

uniform mat4 u_Model;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_Model * vec4(a_Position, 0.0f, 1.f);
    v_UV = a_UV;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_UV;

uniform sampler2D u_Image;
uniform vec3 u_SpriteColor;

void main()
{          
    o_Color = vec4(u_SpriteColor, 1.f) * texture(u_Image, v_UV);
}
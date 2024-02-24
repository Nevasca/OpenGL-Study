#shader vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;

out vec2 v_UV;
out vec4 v_ParticleColor;

uniform mat4 u_Projection;
uniform vec2 u_Offset;
uniform vec4 u_Color;

void main()
{
    float scale = 10.f;
    gl_Position = u_Projection * vec4(a_Position * scale + u_Offset, 0.f, 1.f);

    v_UV = a_UV;
    v_ParticleColor = u_Color;
}

#shader fragment
#version 330 core

out vec4 o_Color;

in vec2 v_UV;
in vec4 v_ParticleColor;

uniform sampler2D u_Sprite;

void main()
{
    o_Color = texture(u_Sprite, v_UV) * v_ParticleColor;
}
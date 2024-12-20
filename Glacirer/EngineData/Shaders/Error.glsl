#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceModelMatrix;    

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * a_InstanceModelMatrix * a_Position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

void main()
{          
    o_Color = vec4(1.f, 0.f, 1.f, 1.f);
}
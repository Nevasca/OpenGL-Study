#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceModelMatrix;    

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * a_InstanceModelMatrix * a_Position; // Instancing approach
    // gl_Position = u_Proj * u_View * u_Model * a_Position; // Uniform approach
    
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

void main()
{          
    o_Color = vec4(0.5f, 0.5f, 0.5f, 1.f);
}
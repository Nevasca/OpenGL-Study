#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceModelMatrix;    

uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * a_InstanceModelMatrix * a_Position; // Instancing approach        
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

// Material
uniform vec4 u_OutlineColor;

void main()
{          
    o_Color = u_OutlineColor;
}
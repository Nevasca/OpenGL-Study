#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceModelMatrix;    

out vec2 v_TexCoord;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * a_InstanceModelMatrix * a_Position; // Instancing approach
    // gl_Position = u_Proj * u_View * u_Model * a_Position; // Uniform approach
    
    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

// Material
uniform vec4 u_Color;
uniform sampler2D u_Diffuse;

void main()
{          
    //o_Color = vec4(0.5f, 0.5f, 0.5f, 1.f);
    o_Color = texture(u_Diffuse, v_TexCoord) + u_Color;
}
#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;    

uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 v_TexCoord;

void main()
{
    gl_Position = u_Proj * u_View * vec4(a_Position, 1.f);

    v_TexCoord = a_Position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec3 v_TexCoord;

// Material
uniform samplerCube u_Skybox;

void main()
{
    o_Color = texture(u_Skybox, v_TexCoord);
}
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

out vec2 v_TexCoord;

void main()
{
    gl_Position = projection * view * a_InstanceModelMatrix * a_Position;

    v_TexCoord = a_TexCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Diffuse;
uniform int u_RenderingMode;

in vec2 v_TexCoord;

const int OPAQUE = 0;
const int ALPHA_CUTOUT = 1;
const int TRANSPARENT = 2;

void main()
{
    vec4 diffuseTextureColor = texture(u_Diffuse, v_TexCoord);
 
    if(u_RenderingMode == ALPHA_CUTOUT && diffuseTextureColor.a < 0.1f)
    {
        discard;
    }

    // gl_FragDepth = gl_FragCoord.z;
}
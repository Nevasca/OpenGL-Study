#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;    

out vec2 v_TexCoord; // v suffix stands for 'varying'. It's the term used for outputting data from vertex shader to fragment shader

uniform mat4 u_MVP; // Model View Projection matrix

void main()
{
    gl_Position = u_MVP * position; // Apply MVP to position to transform it on Normalized Device Coordinates (NDC, from -1 to 1)
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{          
    vec4 texColor = texture(u_Texture, v_TexCoord);  
//    color = u_Color;
    color = texColor;
}
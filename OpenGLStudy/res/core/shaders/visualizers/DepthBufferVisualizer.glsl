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
    gl_Position = u_Proj * u_View * a_InstanceModelMatrix * a_Position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

uniform float u_NearPlane;
uniform float u_FarPlane;

float LinearizeDepth(float depth);

void main()
{
    // Non-linear depth
    //o_Color = vec4(vec3(gl_FragCoord.z), 1.f);

    // Because most of linearized depth values will be above 1.f and displayed as white
    // we divide by far to convert it to the range [0,1]
    float linearDepth = LinearizeDepth(gl_FragCoord.z) / u_FarPlane; 
    o_Color = vec4(vec3(linearDepth), 1.f);
}

// Convert a non linear depth to linear
float LinearizeDepth(float depth)
{    
    // We first need to make it from [0,1] to ndc (normalized device coordinates) [-1, 1]
    float z = depth * 2.f -1.f;

    // Apply the inverse transformation to retrieve linear depth
    return (2.f * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}
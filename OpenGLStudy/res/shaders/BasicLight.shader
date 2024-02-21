#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_FragPosition;

uniform mat4 u_Model;
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_Position;

    // Normal attribute is on local space, we need to convert it to world by using the model matrix
    // But since normal is a direction, it doesn't make sense to translate it, so we cut it by casting to a mat3
    // We also need to take care of non-uniform scale so it doesn't mess with the actual direction, hence the transpose of inverse    
    // Inversing matrices is a costly operation for shaders, a better approach would be calculating it on CPU side and send as uniform, instead of calculating it for each vertex
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_FragPosition = vec3(u_Model * a_Position);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Normal;
in vec3 v_FragPosition;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

void main()
{       
    // Ambient lighting   
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * u_LightColor;

    // Diffuse lighting
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_LightPosition - v_FragPosition);
    float diffValue = max(dot(v_Normal, lightDirection), 0.f); // If greater than 90° it becomes negative and we don't want that, hence max()
    vec3 diffuse = diffValue * u_LightColor;

    // Speculat lighting
    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal); // We do -lightDirection because reflect() expects a direction to point FROM the light source, the var is currently the opposite    
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.f), 32); // 32 is the shininnes value of the highlight, the higher the more it reflects the light instead of scattering around
    vec3 specular = specularStrength * specularValue * u_LightColor;

    // vec3 result = ambient * u_ObjectColor;
    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;

    o_Color = vec4(result, 1.f);
}

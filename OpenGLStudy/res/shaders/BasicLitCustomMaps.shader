#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

out vec3 v_Normal;
out vec2 v_UV;
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
    v_UV = a_UV;
    v_FragPosition = vec3(u_Model * a_Position);
}

#shader fragment
#version 330 core

// We can create structs to organize uniforms
// it will react as namespace on C++ side, like setting a uniform of name "u_Material.ambient"
// Sampler2D can't be instantiated, using it inside struct as uniform is fine, but we can't instantiate it like a return value of a function,
// otherwise it could throw strange errors
struct Material
{
    // removed the ambient color, since is equal to the diffuse color anyways
    sampler2D diffuse; // diffuse map for color of the face under diffuse light. The desired surface color
    sampler2D specular; // color map of the specular highlight on the surface
    sampler2D emission;
    float shininess; // impacts the scattering/radius of the specular highlight
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) out vec4 o_Color;

in vec3 v_Normal;
in vec2 v_UV;
in vec3 v_FragPosition;

uniform Material u_Material;
uniform Light u_Light;
uniform vec3 u_ViewPosition;

void main()
{       
    // Ambient lighting   
    vec3 ambient = u_Light.ambient * vec3(texture(u_Material.diffuse, v_UV));

    // Diffuse lighting
    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(u_Light.position - v_FragPosition);
    float diffValue = max(dot(v_Normal, lightDirection), 0.f); // If greater than 90° it becomes negative and we don't want that, hence max()
    vec3 diffuse = u_Light.diffuse * diffValue * vec3(texture(u_Material.diffuse, v_UV));

    // Specular lighting
    vec3 viewDirection = normalize(u_ViewPosition - v_FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal); // We do -lightDirection because reflect() expects a direction to point FROM the light source, the var is currently the opposite    
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.f), u_Material.shininess); // Shininess is the value of the highlight, the higher the more it reflects the light instead of scattering around
    vec3 specular = u_Light.specular * specularValue * vec3(texture(u_Material.specular, v_UV));
    
    // Emission
    vec3 emission = vec3(texture(u_Material.emission, v_UV));

    vec3 result = ambient + diffuse + specular + emission;

    o_Color = vec4(result, 1.f);
}

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
// Sampler2D can't be instantiated, using it inside struct as uniform is fine, but we can't instantiate it like a return value of a function or pass as param,
// otherwise it could throw strange errors
struct Material
{
    // removed the ambient color, since is equal to the diffuse color anyways
    sampler2D diffuse; // diffuse map for color of the face under diffuse light. The desired surface color
    sampler2D specular; // color map of the specular highlight on the surface
    sampler2D emission;
    float shininess; // impacts the scattering/radius of the specular highlight
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) out vec4 o_Color;

in vec3 v_Normal;
in vec2 v_UV;
in vec3 v_FragPosition;

uniform Material u_Material;
uniform DirLight u_DirLight;
#define NR_POINT_LIGHTS 4 // define number of lights we have on our scene
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLight;
uniform vec3 u_ViewPosition;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPosition - v_FragPosition);

    vec3 result = CalcDirLight(u_DirLight, normal, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(u_PointLights[i], normal, v_FragPosition, viewDir);
    }

    result += CalcSpotLight(u_SpotLight, normal, v_FragPosition, viewDir);

    o_Color = vec4(result, 1.f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // Ambient lighting   
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_UV));

    // Diffuse lighting
    vec3 lightDir = normalize(-light.direction); // Negate because original direction is from light towards frag, we need opposite
    float diffValue = max(dot(normal, lightDir), 0.f); // If greater than 90° it becomes negative and we don't want that, hence max()
    vec3 diffuse = light.diffuse * diffValue * vec3(texture(u_Material.diffuse, v_UV));

    // Specular lighting
    vec3 reflectDirection = reflect(light.direction, normal); // reflect() expects a direction to point FROM the light source
    float specularValue = pow(max(dot(viewDir, reflectDirection), 0.f), u_Material.shininess); // Shininess is the value of the highlight, the higher the more it reflects the light instead of scattering around
    vec3 specular = light.specular * specularValue * vec3(texture(u_Material.specular, v_UV));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Point light attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    // Ambient lighting   
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_UV));
    ambient *= attenuation; // We could leave ambient alone, but for multiple light source it would start to stack up

    // Diffuse lighting
    vec3 lightDir = normalize(light.position - fragPos);
    float diffValue = max(dot(normal, lightDir), 0.f); // If greater than 90° it becomes negative and we don't want that, hence max()
    vec3 diffuse = light.diffuse * diffValue * vec3(texture(u_Material.diffuse, v_UV));
    diffuse *= attenuation;

    // Specular lighting
    vec3 reflectDirection = reflect(-lightDir, normal); // reflect() expects a direction to point FROM the light source
    float specularValue = pow(max(dot(viewDir, reflectDirection), 0.f), u_Material.shininess); // Shininess is the value of the highlight, the higher the more it reflects the light instead of scattering around
    vec3 specular = light.specular * specularValue * vec3(texture(u_Material.specular, v_UV));
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    // To smooth edge
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.f, 1.f);

    // Ambient lighting
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, v_UV)); // Leave ambient light alone without * intensity

    // Diffuse lighting
    float diffValue = max(dot(normal, lightDir), 0.f); // If greater than 90° it becomes negative and we don't want that, hence max()
    vec3 diffuse = light.diffuse * diffValue * vec3(texture(u_Material.diffuse, v_UV));
    diffuse *= intensity;

    // Specular lighting
    vec3 reflectDirection = reflect(light.direction, normal); // reflect() expects a direction to point FROM the light source
    float specularValue = pow(max(dot(viewDir, reflectDirection), 0.f), u_Material.shininess); // Shininess is the value of the highlight, the higher the more it reflects the light instead of scattering around
    vec3 specular = light.specular * specularValue * vec3(texture(u_Material.specular, v_UV));
    specular *= intensity;

    return (ambient + diffuse + specular);
}

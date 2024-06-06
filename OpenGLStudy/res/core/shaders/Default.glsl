#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceModelMatrix;    

out VS_OUT
{
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPosition;
} vsOut;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_View * a_InstanceModelMatrix * a_Position; // Instancing approach

    // Set pointSize when rendering with GL_POINTS
    //gl_PointSize = gl_Position.z;
    
    vsOut.TexCoord = a_TexCoord;
    
    // Normal attribute is on local space, we need to convert it to world by using the model matrix
    // But since normal is a direction, it doesn't make sense to translate it, so we cut it by casting to a mat3
    // We also need to take care of non-uniform scale so it doesn't mess with the actual direction, hence the transpose of inverse    
    vsOut.Normal = mat3(transpose(inverse(a_InstanceModelMatrix))) * a_Normal;
    vsOut.FragPosition = vec3(a_InstanceModelMatrix * a_Position);
}

#shader fragment
#version 330 core

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    
    float intensity;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    
    vec3 diffuse;
    vec3 specular;
    
    float intensity;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
    
    vec3 diffuse;
    vec3 specular;
    
    float intensity;
};

struct AmbientLight
{
    vec3 color;
};

layout(location = 0) out vec4 o_Color;

in VS_OUT
{
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPosition;
} inFrag;

#define MAX_DIRECTIONAL_LIGHTS 3
#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 20

const int OPAQUE = 0;
const int ALPHA_CUTOUT = 1;
const int TRANSPARENT = 2;

// Global Lighting
uniform vec3 u_ViewPosition;
uniform AmbientLight u_AmbientLight;
uniform int u_TotalDirectionalLights;
uniform DirectionalLight u_DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int u_TotalPointLights;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform int u_TotalSpotLights;
uniform SpotLight u_SpotLights[MAX_SPOT_LIGHTS];

// Global Environment
uniform samplerCube u_Skybox;

// Material
uniform vec4 u_Color;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;
uniform float u_ReflectionValue;
uniform int u_RenderingMode;

int materialShininess = 32; // TODO: create material system

vec3 ComputeReflection(vec3 normal, vec3 viewDir);
vec3 ComputeRefraction(vec3 normal, vec3 viewDir);
vec3 ComputeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 baseColor);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor);
vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor);
vec3 ComputeAmbientLight(vec3 baseColor);

void main()
{
    vec4 diffuseTextureColor = texture(u_Diffuse, inFrag.TexCoord);
 
    if(u_RenderingMode == ALPHA_CUTOUT && diffuseTextureColor.a < 0.1f)
    {
        discard;
    }

    vec3 normal = normalize(inFrag.Normal);
    vec3 viewDir = normalize(u_ViewPosition - inFrag.FragPosition);

    vec3 baseColor = diffuseTextureColor.rgb + u_Color.rgb;
    baseColor += ComputeReflection(normal, viewDir);
    
    vec3 result = ComputeAmbientLight(baseColor);
    
    for(int i = 0; i < u_TotalDirectionalLights; i++)
    {
        result += ComputeDirectionalLight(u_DirectionalLights[i], normal, viewDir, baseColor);
    }
    
    for(int i = 0; i < u_TotalPointLights; i++)
    {
        result += ComputePointLight(u_PointLights[i], normal, inFrag.FragPosition, viewDir, baseColor);
    }
    
    for(int i = 0; i < u_TotalSpotLights; i++)
    {
        result += ComputeSpotLight(u_SpotLights[i], normal, inFrag.FragPosition, viewDir, baseColor);
    }
    
    if(u_RenderingMode == OPAQUE)
    {
        o_Color = vec4(result, 1.f);
    }
    else
    {
        o_Color = vec4(result, diffuseTextureColor.a);
    }
}

vec3 ComputeReflection(vec3 normal, vec3 viewDir)
{
    // View dir passed param is from fragmento to view, for reflection we need from view to frag, so we negate here
    vec3 reflectionDir = reflect(-viewDir, normal);
    vec3 reflectionColor = texture(u_Skybox, reflectionDir).rgb * u_ReflectionValue;

    return reflectionColor;
}

// TODO: this is an example using from air to glass, implement receiving ratio from material for customization
vec3 ComputeRefraction(vec3 normal, vec3 viewDir)
{
    // From air to glass
    float ratio = 1.f / 1.52f;

    // View dir passed param is from fragmento to view, for reflection we need from view to frag, so we negate here
    vec3 refractionDir = refract(-viewDir, normal, ratio);
    vec3 refractionColor = texture(u_Skybox, refractionDir).rgb * u_ReflectionValue; // TODO: Using reflectionValue, change for refractionValue

    return refractionColor;
}

vec3 ComputeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 baseColor)
{
    // Diffuse
    vec3 lightDir = normalize(-light.direction);
    float diffuseValue = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diffuseValue * baseColor;
    
    // Specular
    vec3 reflectDirection = reflect(light.direction, normal);    
    float specularValue = pow(max(dot(viewDir, reflectDirection), 0.f), materialShininess);
    vec3 specular = light.specular * specularValue * vec3(texture(u_Specular, inFrag.TexCoord));
    
    return (diffuse + specular) * light.intensity;    
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor)
{
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    if(attenuation <= 0.f)
    {
        return vec3(0.f);
    }
    
    // Diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseValue = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diffuseValue * baseColor;
    diffuse *= attenuation;
    
    // Specular
    vec3 reflectDirection = reflect(-lightDir, normal);
    float specularValue = pow(max(dot(reflectDirection, viewDir), 0), materialShininess);
    vec3 specular = light.specular * specularValue * vec3(texture(u_Specular, inFrag.TexCoord));
    specular *= attenuation;
    
    return (diffuse + specular) * light.intensity;
}

vec3 ComputeSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor)
{
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    if(attenuation <= 0.f)
    {
        return vec3(0.f);
    }
    
    vec3 lightDir = normalize(light.position - fragPos);
    
    float theta = dot(lightDir, normalize(-light.direction));
    // To smooth edge
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.f, 1.f);
    intensity *= attenuation;
    
    // Diffuse
    float diffuseValue = max(dot(normal, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diffuseValue * baseColor;
    diffuse *= intensity;
    
    // Specular
    vec3 reflectDirection = reflect(light.direction, normal);
    float specularValue = pow(max(dot(viewDir, reflectDirection), 0.f), materialShininess);
    vec3 specular = light.specular * specularValue * vec3(texture(u_Specular, inFrag.TexCoord));
    specular *= intensity;
    
    return (diffuse + specular) * light.intensity;
}

vec3 ComputeAmbientLight(vec3 baseColor)
{
    return baseColor * u_AmbientLight.color;
}
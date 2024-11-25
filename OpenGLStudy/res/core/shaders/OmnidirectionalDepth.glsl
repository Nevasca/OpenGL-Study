#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_InstanceModelMatrix;

out vec2 v_TexCoord;

void main()
{
    // Since the geometry shader will transform all vertices in all light space coordinates
    // we only need to transform in world space during the vertex shader
    gl_Position = a_InstanceModelMatrix * a_Position;

    v_TexCoord = a_TexCoord;
}

#shader geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

layout (std140) uniform OmnidirectionalLightMatrices
{
    mat4 viewProjectionMatrices[6];
};

in vec2 v_TexCoord[];

out vec2 g_TexCoord;
out vec4 v_FragPos;

void main()
{
    for(int cubemapFace = 0; cubemapFace < 6; cubemapFace++)
    {
        // Set what face of the cubemap to render
        gl_Layer = cubemapFace;

        for(int vertice = 0; vertice < 3; vertice++)
        {
            v_FragPos = gl_in[vertice].gl_Position;
            g_TexCoord = v_TexCoord[vertice];
            gl_Position = viewProjectionMatrices[cubemapFace] * v_FragPos;
            EmitVertex();
        }

        EndPrimitive();
    }
}

#shader fragment
#version 330 core

#define MAX_POINT_LIGHTS 20

in vec4 v_FragPos;
in vec2 g_TexCoord;

uniform sampler2D u_Diffuse;
uniform int u_RenderingMode;

const int OPAQUE = 0;
const int ALPHA_CUTOUT = 1;
const int TRANSPARENT = 2;

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

layout (std140) uniform LightingPoints
{
    PointLight pointLights[MAX_POINT_LIGHTS];
};

layout (std140) uniform Camera
{
    float nearPlane;
    float farPlane;
};

void main()
{
    if(u_RenderingMode == ALPHA_CUTOUT)
    {
        vec4 diffuseTextureColor = texture(u_Diffuse, g_TexCoord);

        if(diffuseTextureColor.a < 0.1f)
        {
            discard;
        }
    }

    // TODO: assuming only one point light and first one
    vec3 lightPosition = pointLights[0].position;
    
    float lightDistance = length(v_FragPos.xyz - lightPosition);

    // Map to [0:1] range
    lightDistance = lightDistance / farPlane;

    // write as modified depth
    gl_FragDepth = lightDistance;
}

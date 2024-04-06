#include "LegacyMesh.h"

#include "Renderer.h"
#include "LegacyShader.h"
#include "LegacyTexture.h"


#include "LegacyVertexBufferLayout.h"

LegacyMesh::LegacyMesh(std::vector<LegacyVertex> vertices, std::vector<unsigned> indices, const std::vector<std::shared_ptr<LegacyTexture>>& textures)
    : m_Textures(textures)
{
    m_VAO = std::make_unique<LegacyVertexArray>();

    m_VBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(LegacyVertex));

    LegacyVertexBufferLayout layout{};
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);

    m_IBO = std::make_unique<LegacyIndexBuffer>(indices.data(), indices.size());
}

void LegacyMesh::Draw(LegacyShader& shader)
{
    shader.Bind();

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    
    for(unsigned int i = 0; i < m_Textures.size(); i++)
    {
        m_Textures[i]->Bind(i);

        // We don't know how many textures the mesh has, so we are using a convention
        // for naming them as texture_diffuse or texture_specular and set as many as the shader accepts
        std::string number;
        const std::string& name = m_Textures[i]->GetType();
        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }

        std::string uniformName = "u_Material." + name + number;
        shader.SetUniform1i(uniformName, i);
    }

    // glActiveTexture(GL_TEXTURE0); // Do we need that?

    Renderer renderer{};
    renderer.Draw(*m_VAO, *m_IBO, shader);
}

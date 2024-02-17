#include "TestStaticBatchRendering.h"

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace tests
{
    TestStaticBatchRendering::TestStaticBatchRendering()
    {
        float vertices[]
        {
            // Position       Color (normalized RGBA)         UV       Texture Id
            100.f, 300.f,     0.18f, 0.6f, 0.96f, 1.f,     0.f, 0.f,      0.f,  
            200.f, 300.f,     0.18f, 0.6f, 0.96f, 1.f,     1.f, 0.f,      0.f,
            200.f, 400.f,     0.18f, 0.6f, 0.96f, 1.f,     1.f, 1.f,      0.f,
            100.f, 400.f,     0.18f, 0.6f, 0.96f, 1.f,     0.f, 1.f,      0.f,

            300.f, 300.f,     1.f, 0.93f, 0.24f, 1.f,      0.f, 0.f,      1.f,
            400.f, 300.f,     1.f, 0.93f, 0.24f, 1.f,      1.f, 0.f,      1.f,
            400.f, 400.f,     1.f, 0.93f, 0.24f, 1.f,      1.f, 1.f,      1.f,
            300.f, 400.f,     1.f, 0.93f, 0.24f, 1.f,      0.f, 1.f,      1.f
        };

        unsigned int indexes[]
        {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };
        
        m_VertexArray = std::make_unique<VertexArray>();

        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 9 * sizeof(float));

        VertexBufferLayout layout{};
        layout.Push<float>(2);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indexes, 12);

        m_Shader = std::make_unique<Shader>("res/shaders/BasicBatch.shader");
        
        m_TextureA = std::make_unique<Texture>("res/textures/FancyPigeon.png");
        m_TextureB = std::make_unique<Texture>("res/textures/UnownJar.png");

        m_Shader->Bind();
        
        m_TextureA->Bind(0);
        m_TextureB->Bind(1);

        int samplers [2] {0, 1};
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);

        m_Proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f);
        m_View = glm::mat4{1};
    }

    TestStaticBatchRendering::~TestStaticBatchRendering()
    { }

    void TestStaticBatchRendering::OnRender()
    {
        Renderer renderer{};

        m_Shader->Bind();
        
        m_Shader->SetUniformMat4f("u_MVP", m_Proj * m_View);

        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }
}

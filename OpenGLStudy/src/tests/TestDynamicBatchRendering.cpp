#include "TestDynamicBatchRendering.h"

#include <array>

#include "LegacyIndexBuffer.h"
#include "Renderer.h"
#include "LegacyShader.h"
#include "LegacyTexture.h"
#include "LegacyVertexArray.h"
#include "LegacyVertexBuffer.h"
#include "LegacyVertexBufferLayout.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "imgui/imgui.h"

namespace tests
{
    struct Vec2
    {
        float X, Y;
    };

    struct Vec4
    {
        float X, Y, Z, W;
    };

    // Make it clearer our vertex data
    // and also easier so we can set the desired values for the vertex
    // like Vertex v{}; v.Positions = {100.f, 300.f};
    struct Vertex
    {
        Vec2 Position;
        Vec4 Color;
        Vec2 TexCoords;
        float TexId;
    };
    
    TestDynamicBatchRendering::TestDynamicBatchRendering()
    {
        // Nothing changes for index buffer
        // Since drawing quads follows the same pattern (0,1,2,2,3,0),
        // we could create a for loop to populate the indexes with all 250 quads indexes we are reserving on vertex buffer
        // 1000 vertex / 4 vertex per quad = 250 quads
        // To keep it simple, we will leave the 2 quads as it is
        unsigned int indexes[]
        {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };
        
        m_VertexArray = std::make_unique<LegacyVertexArray>();

        // Passing the data as nullptr means we are not providing the data right now, we just want to allocate the required VRAM
        // Allocating VRAM for 1,000 vertices. We can't exceed the specified value, but we have no obligation of providing all the 1,000 later
        m_VertexBuffer = std::make_unique<LegacyVertexBuffer>(nullptr, sizeof(Vertex) * 1000, true);

        LegacyVertexBufferLayout layout{};
        layout.Push<float>(2);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(1);
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<LegacyIndexBuffer>(indexes, 12);

        m_Shader = std::make_unique<LegacyShader>("res/shaders/BasicBatch.shader");
        
        m_TextureA = std::make_unique<LegacyTexture>("res/textures/FancyPigeon.png");
        m_TextureB = std::make_unique<LegacyTexture>("res/textures/UnownJar.png");

        m_Shader->Bind();
        
        m_TextureA->Bind(0);
        m_TextureB->Bind(1);

        int samplers [2] {0, 1};
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);

        m_Proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f);
        m_View = glm::mat4{1};
    }

    TestDynamicBatchRendering::~TestDynamicBatchRendering()
    { }

    static std::array<Vertex, 4> CreateQuad(float X, float Y, float TextureID)
    {
        float Size = 100.f;

        // Create quad vertices assuming origin (pivot) is bottom left
        Vertex v0{};
        v0.Position = { X, Y };
        v0.Color = { 0.18f, 0.6f, 0.96f, 1.f };
        v0.TexCoords = { 0.f, 0.f };
        v0.TexId = TextureID;

        Vertex v1{};
        v1.Position = { X + Size, Y };
        v0.Color = { 0.18f, 0.6f, 0.96f, 1.f };
        v1.TexCoords = { 1.f, 0.f };
        v1.TexId = TextureID;

        Vertex v2{};
        v2.Position = { X + Size, Y + Size };
        v0.Color = { 0.18f, 0.6f, 0.96f, 1.f };
        v2.TexCoords = { 1.f, 1.f };
        v2.TexId = TextureID;

        Vertex v3{};
        v3.Position = { X, Y + Size };
        v0.Color = { 0.18f, 0.6f, 0.96f, 1.f };
        v3.TexCoords = { 0.f, 1.f };
        v3.TexId = TextureID;

        return {v0, v1, v2, v3};
    }

    void TestDynamicBatchRendering::OnRender()
    {
        Renderer renderer{};

        // Set dynamic vertex buffer
        m_VertexBuffer->Bind();
        auto Q0 = CreateQuad(m_translationA.x, m_translationA.y, 0.f);
        auto Q1 = CreateQuad(m_translationB.x, m_translationB.y, 1.f);

        // Copy quads memory into vertices
        Vertex vertices[8];
        memcpy(vertices, Q0.data(), Q0.size() * sizeof(Vertex));
        memcpy(vertices + Q0.size(), Q1.data(), Q1.size() * sizeof(Vertex));
        
        // Similar to glBufferData, but instead of allocating, it just send the data to the already allocated buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", m_Proj * m_View);

        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void TestDynamicBatchRendering::OnImGuiRender()
    {
        ImGui::DragFloat2("Quad A", &m_translationA.x, 0.3f);
        ImGui::DragFloat2("Quad B", &m_translationB.x, 0.3f);
    }
}

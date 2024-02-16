#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "Test.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace tests
{
    class TestTexturedSquare : public Test
    {
    public:

        TestTexturedSquare();
        ~TestTexturedSquare() override;

        void OnUpdate(float DeltaTime) override;
        void OnRender(const Renderer& Renderer) override;
        void OnImGuiRender() override;

    private:

        float positions[16] {
            -50.f, -50.f, 0.0f, 0.0f, // 0 // Bottom left vertex
             50.f, -50.f, 1.0f, 0.0f, // 1 // Bottom right
             50.f,  50.f, 1.0f, 1.0f, // 2 // Top right
            -50.f,  50.f, 0.0f, 1.0f  // 3 // Top left
        };

        // We could use unsigned char, for instance,
        // but for more complex models it will require more bytes
        // but it must be an unsigned type
        unsigned int indices[6] {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va{};
        VertexBuffer vb;
        IndexBuffer ib;
        Shader shader;
        Texture texture;
        
        glm::mat4 proj;
        glm::mat4 view;
        glm::vec3 translationA{glm::vec3{200.f, 200.f, 0.f}};
        glm::vec3 translationB{glm::vec3{400.f, 200.f, 0.f}};
    };
}

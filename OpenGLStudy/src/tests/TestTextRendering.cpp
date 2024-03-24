#include "TestTextRendering.h"

#include "Core.h"

#include <ft2build.h>
#include <iostream>

#include "IndexBuffer.h"
#include "LegacyShader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "imgui/imgui.h"

#include FT_FREETYPE_H

namespace tests
{
    TestTextRendering::TestTextRendering()
    {
        FT_Library freeType;

        // Returns non 0 if error
        if(FT_Init_FreeType(&freeType))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType library\n";
            return;
        }

        FT_Face face; // face is how FreeType calls a font
        if(FT_New_Face(freeType, "res/breakout/fonts/arial.ttf", 0, &face))
        {
            std::cout << "ERROR:FREETYPE: Could not load the font\n";
            return;
        }

        // Define pixel font size we'd like to extract from this face
        // Setting width to 0 lets the face dynamically calculate it based on given height
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disable byte-alignment restriction
        // since we will use a single byte to represent the colors of a texture
        GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

        // Get all 128 characters of the ASCII set
        for(unsigned char c = 0; c < 128; c++)
        {
            // Load/sets glyph char as active glyph
            // FT_LOAD_RENDER flag tells FreeType an 8-bit grayscale bitmap
            if(FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYPE: Failed to load glyph '" << c << "'\n";
                continue;
            }

            // Generate texture for the glyph
            unsigned int textureId;
            GLCall(glGenTextures(1, &textureId));
            GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
            GLCall(glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED, // glyph bitmap is a 8-bit grayscale. Each byte will correspond to the texture red color component
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            ));

            // Set texture options
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            // Store character for later use
            Character character{
                textureId,
                glm::ivec2{face->glyph->bitmap.width, face->glyph->bitmap.rows},
                glm::ivec2{face->glyph->bitmap_left, face->glyph->bitmap_top},
                static_cast<unsigned int>(face->glyph->advance.x)
            };

            m_Characters.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(freeType);

        m_VAO = std::make_unique<VertexArray>();
        
        m_VBO = std::make_unique<VertexBuffer>(nullptr, 4 * 4 * sizeof(float), true); // reserving for 4 vertices of 4 floats (a quad)
        VertexBufferLayout layout{};
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VBO, layout);

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<LegacyShader>("res/shaders/Text.shader");
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Projection", glm::ortho(0.f, 960.f, 0.f, 540.f));
        m_Shader->SetUniform1i("u_Text", 0);
    }

    TestTextRendering::~TestTextRendering()
    {
        GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
    }

    void TestTextRendering::OnUpdate(float DeltaTime)
    {
    }

    void TestTextRendering::OnRender()
    {
        RenderText(*m_Shader, m_Text, m_Position.x, m_Position.y, m_Scale, m_Color);
    }

    void TestTextRendering::RenderText(const LegacyShader& Shader, const std::string& Text, float X, float Y, float Scale, const glm::vec3& Color)
    {
        m_VAO->Bind();

        m_Shader->Bind();
        m_Shader->SetUniform3f("u_TextColor", Color);

        GLCall(glActiveTexture(GL_TEXTURE0));

        // Iterate through all characters
        std::string::const_iterator c;
        for(c = Text.begin(); c != Text.end(); c++)
        {
            Character character = m_Characters[*c];

            float xPos = X + character.Bearing.x * Scale;
            float yPos = Y - (character.Size.y - character.Bearing.y) * Scale; // some characters are positioned slightly below the baseline

            float w = character.Size.x * Scale;
            float h = character.Size.y * Scale;

            float vertices[] =
            {
                // Pos                 UV (flipped vertically)
                xPos, yPos,         0.f, 1.f,
                xPos + w, yPos,     1.f, 1.f,
                xPos + w, yPos + h, 1.f, 0.f,
                xPos, yPos + h,     0.f, 0.f,
            };

            GLCall(glBindTexture(GL_TEXTURE_2D, character.TextureID));

            // m_VBO->Bind(); // Do we need that?
            m_VBO->SetSubData(vertices, sizeof(vertices)); // Sets the dynamic vertices data
            // m_VBO->Unbind(); // Do we need that?

            GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));

            // Advance cursors for the next glyph
            // Advance is number of 1/64 pixels, we bitshift by 6 to get value in pixel (2 ^ 6 = 64)
            X += (character.Advance >> 6) * Scale;
        }
    }
    
    void TestTextRendering::OnImGuiRender()
    {
        ImGui::InputText("Text", m_Text, IM_ARRAYSIZE(m_Text));
        ImGui::InputFloat("Scale", &m_Scale);
        ImGui::InputFloat2("Position", &m_Position.x);
        ImGui::ColorEdit3("Color", &m_Color.r);
    }
}

#include "TestFramebuffer.h"

#include <iostream>

#include "LegacyCamera.h"
#include "Core.h"
#include "LegacyFlyCameraController.h"
#include "LegacyModel.h"
#include "LegacyShader.h"
#include "imgui/imgui.h"

namespace tests
{
    TestFramebuffer::TestFramebuffer()
    {
        // The process to create a framebuffer is similar to other objects: generate one, bind it and do some operations
        GLCall(glGenFramebuffers(1, &m_FBO));
        
        // Binding by using GL_FRAMEBUFFER target all the next read and write framebuffer operations will affect the currently bound
        // We can also bind to a read or write target by using GL_READ_FRAMEBUFFER (used for glReadPixels for example) or GL_DRAW_FRAMEBUFFER (rendering destination, clearing)
        // Most of the time we want GL_FRAMEBUFFER
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

        // We must setup some attachments to the framebuffer so it can be completed
        // Attachment is a memory location that can act as a buffer for the framebuffer, think of it as an image

        // Creating a texture attachment, all rendering commands will write to the texture
        // We can use this texture containing the render output on a shader and do some cool stuff
        GLCall(glGenTextures(1, &m_TextureColorBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer));

        // We set the dimension equal to screen size (although not required) and the data as null, meaning we are just reserving memory for it
        // filling the texture will happen when rendering to the framebuffer
        // if we wanted to render the whole screen on a smaller texture, we need to call glViewport before rendering to the framebuffer
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 540, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
        // We don't care about wrapping methods or mipmapping, we won't be needing them most of the cases
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind texture

        // Attach the texture to the framebuffer
        // target: type of the framebuffer
        // attachment: type of attachment (note using suffix 0, we can attach more the one color attachment)
        // textarget: type of texture
        // texture: the actual texture (id)
        // level: mipmap level
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorBuffer, 0));

        // We can also create attach a depth and a stencil texture to the framebuffer if we want to read from them
        // check more on: https://learnopengl.com/Advanced-OpenGL/Framebuffers
        // since we will not sample from depth or stencil on the implemented postprocessing
        // we can create a renderbuffer object attachment that is faster due some few OpenGL optimizations, but can't be read from them directly
        // we need the depth and stencil values for testing, since we are not sampling from them we use renderbuffer object for that
        GLCall(glGenRenderbuffers(1, &m_RBO));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 540)); // create a renderbuffer as a depth and stencil buffer (with 24 and 8 bits respectively)
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0)); // unbind renderbuffer

        // Attach the renderbuffer with depth and stencil to the framebuffer
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO));
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR:FRAMEBUFFER: Framebuffer is not complete!\n";
        }

        // Make sure to unbind the framebuffer to not accidentally render to the wrong framebuffer 
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        CreateTestWorld();
        CreateScreenQuad();
    }

    TestFramebuffer::~TestFramebuffer()
    {
        // We are done using our custom frame buffer, bind it to the default one again
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        // And delete ours
        GLCall(glDeleteFramebuffers(1, &m_FBO));
        GLCall(glDeleteRenderbuffers(1, &m_RBO));
        GLCall(glDeleteTextures(1, &m_TextureColorBuffer));

        GLCall(glEnable(GL_DEPTH_TEST));
    }

    void TestFramebuffer::Setup(GLFWwindow* Window)
    {
        m_CameraController->Setup();
    }

    void TestFramebuffer::Shutdown(GLFWwindow* Window)
    {
        m_CameraController->Shutdown();
    }

    void TestFramebuffer::OnUpdate(float DeltaTime)
    {
        m_CameraController->Update();
    }

    void TestFramebuffer::OnRender()
    {
        // First pass:
        // Render scene using our custom framebuffer, clearing it first
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glEnable(GL_DEPTH_TEST));
        
        m_MVP = m_Camera->GetViewProjectionMatrix();
        m_ModelShader->Bind();
        m_ModelShader->SetUniformMat4f("u_MVP", m_MVP);
        m_Model->Draw(*m_ModelShader);

        // Second pass:
        // Back to the default framebuffer and render our quad that covers the entire screen
        // using the frame texture result on it
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        GLCall(glClearColor(1.f, 1.f, 1.f, 1.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glDisable(GL_DEPTH_TEST));
        
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer));

        m_PostProcessingShader->Bind();
        m_PostProcessingShader->SetUniform1i("u_PostProcessing.SharpenEnabled", bSharpenEnabled);
        m_PostProcessingShader->SetUniform1i("u_PostProcessing.BlurEnabled", bBlurEnabled);
        m_PostProcessingShader->SetUniform1i("u_PostProcessing.EdgeDetectionEnabled", bEdgeDetectionEnabled);
        m_PostProcessingShader->SetUniform1i("u_PostProcessing.ColorInversionEnabled", bColorInversionEnabled);
        m_PostProcessingShader->SetUniform1i("u_PostProcessing.GrayScaleEnabled", bGrayScaleEnabled);

        m_ScreenQuad->Draw(*m_PostProcessingShader);
    }

    void TestFramebuffer::OnImGuiRender()
    {
        ImGui::Checkbox("Sharpen", &bSharpenEnabled);
        ImGui::Checkbox("Blur", &bBlurEnabled);
        ImGui::Checkbox("Edge Detection", &bEdgeDetectionEnabled);
        ImGui::Checkbox("Color Inversion", &bColorInversionEnabled);
        ImGui::Checkbox("Gray Scale", &bGrayScaleEnabled);
    }

    void TestFramebuffer::CreateTestWorld()
    {
        m_Camera = std::make_shared<LegacyCamera>(960.f, 540.f);
        m_Camera->Position = {10.f, 10.f,15.f};

        m_CameraController = std::make_unique<LegacyFlyCameraController>(m_Camera);

        m_ModelShader = std::make_unique<LegacyShader>("res/shaders/BasicGray.glsl");
        m_ModelShader->Bind();

        m_Model = std::make_unique<LegacyModel>(m_ModelFilePath);
    }

    void TestFramebuffer::CreateScreenQuad()
    {
        // Since we don't need to transform this quad, we can directly use native device position (from -1 to 1) 
        std::vector<LegacyVertex> vertices = {
            LegacyVertex{{-1.f, -1.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f}},
            LegacyVertex{{1.f, -1.f, 0.f}, {1.f, 0.f, 0.f}, {1.f, 0.f}},
            LegacyVertex{{1.f, 1.f, 0.f}, {1.f, 0.f, 0.f}, {1.f, 1.f}},
            LegacyVertex{{-1.f, 1.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 1.f}}
        };

        std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
        
        m_ScreenQuad = std::make_unique<LegacyMesh>(vertices, indices, std::vector<std::shared_ptr<LegacyTexture>>{});

        m_PostProcessingShader = std::make_unique<LegacyShader>("res/shaders/PostProcessing.glsl");
        m_PostProcessingShader->Bind();
        m_PostProcessingShader->SetUniform1i("u_ScreenTexture", 0);
    }
}

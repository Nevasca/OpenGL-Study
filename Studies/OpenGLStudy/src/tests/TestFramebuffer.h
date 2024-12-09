#pragma once

#include <memory>
#include <string>

#include "Test.h"
#include "glm/glm.hpp"

class LegacyMesh;
class LegacyFlyCameraController;
class LegacyCamera;
class LegacyShader;
class LegacyModel;

namespace tests
{
    class TestFramebuffer : public Test
    {
    public:
        TestFramebuffer();
        ~TestFramebuffer() override;

        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;


    private:

        // Just like other objects on OpenGL, like vertex array, vertex buffer... framebuffers also have an id
        unsigned int m_FBO; // framebuffer object
        unsigned int m_TextureColorBuffer; // texture object
        unsigned int m_RBO; // renderbuffer object

        std::unique_ptr<LegacyModel> m_Model{};
        std::string m_ModelFilePath{"res/models/Bridge.fbx"};
        std::unique_ptr<LegacyShader> m_ModelShader;
        glm::mat4 m_MVP;
        std::shared_ptr<LegacyCamera> m_Camera;
        std::unique_ptr<LegacyFlyCameraController> m_CameraController;
        std::unique_ptr<LegacyShader> m_PostProcessingShader;
        std::unique_ptr<LegacyMesh> m_ScreenQuad;
        bool bSharpenEnabled{false};
        bool bBlurEnabled{false};
        bool bEdgeDetectionEnabled{false};
        bool bColorInversionEnabled{false};
        bool bGrayScaleEnabled{false};

        void CreateTestWorld();
        void CreateScreenQuad();
    };
}

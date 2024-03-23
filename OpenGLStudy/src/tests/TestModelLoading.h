#pragma once

#include <memory>
#include <string>

#include "Test.h"
#include "glm/glm.hpp"

class Shader;
class LegacyFlyCameraController;
class LegacyCamera;
class Model;

namespace tests
{
    class TestModelLoading : public Test
    {
    public:
        TestModelLoading();
        ~TestModelLoading() override;

        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<Model> m_Model{};
        std::string m_ModelFilePath{"res/models/Bridge.fbx"};
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_MVP;
        glm::vec3 m_Scale{1.f};
        std::shared_ptr<LegacyCamera> m_Camera;
        std::unique_ptr<LegacyFlyCameraController> m_CameraController;

        void LoadModel();        
    };
}

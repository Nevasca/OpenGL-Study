#pragma once

#include <memory>

#include "legacy/LegacyCamera.h"
#include "legacy/LegacyFlyCameraController.h"
#include "Test.h"
#include "glm/glm.hpp"

class LegacyTexture;
class LegacyShader;
class LegacyIndexBuffer;
class LegacyVertexBuffer;
class LegacyVertexArray;

namespace tests
{
    class TestBasicLightingCasters : public Test
    {
    public:
        TestBasicLightingCasters();
        ~TestBasicLightingCasters() override;

        void Setup(GLFWwindow* Window) override;
        void Shutdown(GLFWwindow* Window) override;
        void OnUpdate(float DeltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:

        std::unique_ptr<LegacyVertexArray> m_CubeVAO;
        std::unique_ptr<LegacyVertexBuffer> m_CubeVBO;
        std::unique_ptr<LegacyIndexBuffer> m_CubeIBO;
        std::unique_ptr<LegacyShader> m_CubeShader;
        std::unique_ptr<LegacyTexture> m_CubeTextureDiff;
        std::unique_ptr<LegacyTexture> m_CubeTextureSpec;
        std::unique_ptr<LegacyTexture> m_CubeTextureEmission;

        std::unique_ptr<LegacyVertexArray> m_LightCubeVAO;
        std::unique_ptr<LegacyVertexBuffer> m_LightCubeVBO;
        std::unique_ptr<LegacyIndexBuffer> m_LightCubeIBO;
        std::unique_ptr<LegacyShader> m_LightCubeShader;

        static constexpr int TOTAL_CUBES = 5;
        glm::vec3 m_Positions[TOTAL_CUBES];
        glm::vec3 m_Scales[TOTAL_CUBES];
        glm::vec3 m_Rotations[TOTAL_CUBES];

        bool m_OrbitLightEnabled{false};

        glm::vec3 m_LightCubeScale{0.2f, 0.2f, 0.2f};
        
        glm::vec3 m_DirectionalLightPosition{1.2f, 1.f, 2.f};
        glm::vec3 m_DirectionalLightRotation{130.f, 0.f, 0.f};
        glm::vec3 m_DirectionalLightAmbientColor{0.2f, 0.2f, 0.2f};
        glm::vec3 m_DirectionalLightDiffuseColor{0.5f, 0.5f, 0.5f};
        glm::vec3 m_DirectionalLightSpecularColor{1.f, 1.f, 1.f};

        // Needs to be the same as on shader, otherwise nothing will show up.
        // Maybe try experimenting having a flag on the PointLight uniform struct like isEnabled and on start up set all array elements to false?
        // And when a light is created keep track of its index and set that to enabled...
        static constexpr int TOTAL_LIGHT_POINTS = 4; 
        glm::vec3 m_PointLightPositions[TOTAL_LIGHT_POINTS];
        glm::vec3 m_PointLightRotations[TOTAL_LIGHT_POINTS];
        glm::vec3 m_PointLightAmbientColors[TOTAL_LIGHT_POINTS];
        glm::vec3 m_PointLightDiffuseColors[TOTAL_LIGHT_POINTS];
        glm::vec3 m_PointLightSpecularColors[TOTAL_LIGHT_POINTS];
        float m_PointLightConstants[TOTAL_LIGHT_POINTS];
        float m_PointLightLinears[TOTAL_LIGHT_POINTS];
        float m_PointLightQuadratics[TOTAL_LIGHT_POINTS];

        glm::vec3 m_SpotLightPosition{1.2f, 2.f, 2.f};
        glm::vec3 m_SpotLightRotation{130.f, 0.f, 0.f};
        glm::vec3 m_SpotLightAmbientColor{0.2f, 0.2f, 0.2f};
        glm::vec3 m_SpotLightDiffuseColor{0.5f, 0.5f, 0.5f};
        glm::vec3 m_SpotLightSpecularColor{1.f, 1.f, 1.f};
        float m_SpotLightCutoff{12.5f};
        float m_SpotLightOuterCutoff{17.5f};

        float m_CubeShininess{32.f};
        
        glm::mat4 m_MVP;

        std::shared_ptr<LegacyCamera> m_Camera;
        std::unique_ptr<LegacyFlyCameraController> m_CameraController;

        void CreateCubes();
        void CreateLightSources();
        glm::mat4 GetRotationMatrix(const glm::vec3& EulerRotation) const;
        glm::vec3 GetForwardVector(const glm::vec3& EulerRotation) const;
    };
}

#include "Basics/Components/PointLightComponent.h"

#include "World.h"
#include "GameObject/GameObject.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

void PointLightComponent::Initialize()
{
    Component::Initialize();

    SetRange(m_Range);

    World& world = GetOwner().GetWorld();
    world.AddPointLight(GetThis());
}

void PointLightComponent::SetAttenuation(float constant, float linear, float quadratic)
{
    m_Attenuation.Constant = constant;
    m_Attenuation.Linear = linear;
    m_Attenuation.Quadratic = quadratic;
}

glm::vec3 PointLightComponent::GetPosition() const
{
    return GetOwnerPosition();
}

void PointLightComponent::SetRange(float range)
{
    m_Range = std::max(range, 0.001f);
    m_Attenuation = Light::CalculateAttenuation(range);
}

glm::mat4 PointLightComponent::GetProjectionMatrix(const Rendering::Resolution& shadowResolution) const
{
    float constexpr near = 1.f;
    float constexpr far = 25.f;
    float constexpr fov = 90.0f;
    
    float aspect = static_cast<float>(shadowResolution.Width)/static_cast<float>(shadowResolution.Height);

    return glm::perspective(glm::radians(fov), aspect, near, far);
}

std::vector<glm::mat4> PointLightComponent::GetViewProjectionMatrices(const Rendering::Resolution& shadowResolution) const
{
    glm::mat4 projection = GetProjectionMatrix(shadowResolution);
    glm::vec3 position = GetPosition();
    
    std::vector<glm::mat4> matrices{};
    matrices.reserve(6);

    // Look at each direction of the point light to be used by cubemap shadow map (right, left, top, bottom, near and far)
    matrices.emplace_back(projection * glm::lookAt(position, position + glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
    matrices.emplace_back(projection * glm::lookAt(position, position + glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
    matrices.emplace_back(projection * glm::lookAt(position, position + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)));
    matrices.emplace_back(projection * glm::lookAt(position, position + glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)));
    matrices.emplace_back(projection * glm::lookAt(position, position + glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f)));
    matrices.emplace_back(projection * glm::lookAt(position, position + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f)));

    return matrices;
}


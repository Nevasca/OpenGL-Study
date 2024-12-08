#pragma once
#include <string>

class GameObject;
class World;

namespace Editor
{
    namespace Sandbox
    {
        class SandboxSceneSpawner
        {
        public:
            static void Spawn(World& world);

        private:
            static std::string SANDBOX_RESOURCES_PATH;
            
            static void SpawnCrates(World& world);
            static void SpawnBridge(World& world);
            static void SpawnWarrior(World& world);
            static void SpawnTransparentObjects(World& world);
            static void SpawnFloor(World& world);
            static void SpawnLights(World& world, GameObject& camera);
        };
    }
}

#pragma once
#include <string>

namespace Glacirer
{
    class GameObject;
    class World;
}

namespace GlacirerEditor
{
    namespace Sandbox
    {
        class SandboxSceneSpawner
        {
        public:
            static void Spawn(Glacirer::World& world);

        private:
            static std::string SANDBOX_RESOURCES_PATH;
            
            static void SpawnCrates(Glacirer::World& world);
            static void SpawnBridge(Glacirer::World& world);
            static void SpawnWarrior(Glacirer::World& world);
            static void SpawnTransparentObjects(Glacirer::World& world);
            static void SpawnFloor(Glacirer::World& world);
            static void SpawnLights(Glacirer::World& world, Glacirer::GameObject& camera);
        };
    }
}

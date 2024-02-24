#pragma once
#include <vector>

#include "GameObject.h"

namespace Breakout
{
    class GameLevel
    {
    public:
        std::vector<GameObject> Bricks;

        GameLevel(const std::shared_ptr<Texture>& solidBlockTexture, const std::shared_ptr<Texture>& blockTexture);
        void Load(const char* filePath, unsigned int levelWidth, unsigned int levelHeight);
        void Draw(const SpriteRenderer& renderer);
        bool IsCompleted() const;
        void Reset();

    private:

        std::shared_ptr<Texture> m_SolidBlockTexture;
        std::shared_ptr<Texture> m_BlockTexture;
        
        void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
    };
}

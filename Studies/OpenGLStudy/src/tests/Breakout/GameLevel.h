#pragma once
#include <vector>

#include "BreakoutGameObject.h"

namespace Breakout
{
    class GameLevel
    {
    public:
        std::vector<BreakoutGameObject> Bricks;

        GameLevel(const std::shared_ptr<LegacyTexture>& solidBlockTexture, const std::shared_ptr<LegacyTexture>& blockTexture);
        void Load(const char* filePath, unsigned int levelWidth, unsigned int levelHeight);
        void Draw(const SpriteRenderer& renderer);
        bool IsCompleted() const;
        void Reset();

    private:

        std::shared_ptr<LegacyTexture> m_SolidBlockTexture;
        std::shared_ptr<LegacyTexture> m_BlockTexture;
        
        void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
    };
}

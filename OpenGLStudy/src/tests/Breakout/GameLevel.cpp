#include "GameLevel.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "Texture.h"

namespace Breakout
{
    GameLevel::GameLevel(const std::shared_ptr<Texture>& solidBlockTexture, const std::shared_ptr<Texture>& blockTexture)
        : m_SolidBlockTexture(solidBlockTexture), m_BlockTexture(blockTexture)
    {
    }

    void GameLevel::Load(const char* filePath, unsigned levelWidth, unsigned levelHeight)
    {
        Bricks.clear();

        unsigned int tileCode;
        std::vector<std::vector<unsigned int>> tileData;

        std::ifstream fileStream(filePath);
        std::string line;

        assert(fileStream);

        while(std::getline(fileStream, line)) // each line of file
        {
            std::istringstream stringStream(line);
            std::vector<unsigned int> row;

            while(stringStream >> tileCode) // each char of the line
            {
                row.push_back(tileCode);                
            }

            tileData.push_back(row);
        }

        assert(!tileData.empty());

        Init(std::move(tileData), levelWidth, levelHeight);
    }

    void GameLevel::Init(std::vector<std::vector<unsigned>> tileData, unsigned int levelWidth, unsigned int levelHeight)
    {
        unsigned int height = tileData.size();
        unsigned int width = tileData[0].size();
        float unitWidth = levelWidth / static_cast<float>(width);
        float unitHeight = levelHeight / height;
        
        Bricks.reserve(height * width);

        for(unsigned int y = 0; y < height; y++)
        {
            for(unsigned int x = 0; x < width; x++)
            {
                glm::vec2 pos{unitWidth * x, unitHeight * y};
                glm::vec2 size{unitWidth, unitHeight};

                const unsigned int tileCode = tileData[y][x]; 
                
                if(tileCode == 1) // solid
                {
                    GameObject brick{pos, size, m_SolidBlockTexture, glm::vec3{0.8f, 0.8f, 0.7f}};
                    brick.IsSolid = true;

                    Bricks.emplace_back(std::move(brick));

                    continue;
                }

                if(tileCode > 1)
                {
                    glm::vec3 color = glm::vec3{1.f};

                    if(tileCode == 2)
                        color = glm::vec3{0.2f, 0.6f, 1.f};
                    else if(tileCode == 3)
                        color = glm::vec3(0.0f, 0.7f, 0.0f);
                    else if(tileCode == 4)
                        color = glm::vec3(0.8f, 0.8f, 0.4f);
                    else if(tileCode == 5)
                        color = glm::vec3(1.0f, 0.5f, 0.0f);

                    Bricks.emplace_back(pos, size, m_BlockTexture, color);
                }
            }
        }
    }

    void GameLevel::Draw(const SpriteRenderer& renderer)
    {
        for(const GameObject& Brick : Bricks)
        {
            if(!Brick.Destroyed)
            {
                Brick.Draw(renderer);
            }
        }
    }

    bool GameLevel::IsCompleted() const
    {
        for(const GameObject& Brick : Bricks)
        {
            if(!Brick.IsSolid && !Brick.Destroyed)
            {
                return false;
            }
        }

        return true;
    }

}

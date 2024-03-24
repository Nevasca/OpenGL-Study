#pragma once

#include <memory>
#include <vector>

#include "EGameState.h"
#include "GameLevel.h"
#include <irrKlang.h>

class Texture;
class LegacyShader;

namespace Breakout
{
    class ParticleGenerator;
    class Ball;
    class SpriteRenderer;
    
    enum EDirection
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };
    
    typedef std::tuple<bool, EDirection, glm::vec2> Collision;

    class Game
    {
    public:
        EGameState State{EGameState::GAME_ACTIVE};
        bool Keys[1024];
        unsigned int Width;
        unsigned Height;

        std::vector<GameLevel> Levels;
        unsigned int Level{0};
        std::shared_ptr<Texture> m_BlockTexture;

        Game(unsigned width, unsigned height);
        ~Game();

        void Init();
        void ProcessInput(float deltaTime);
        void Update(float deltaTime);
        void Render();
        void DoCollisions();

    private:

        const glm::vec2 PLAYER_SIZE{100.f, 20.f};
        const float PLAYER_VELOCITY{500.f};
        const glm::vec2 BALL_INITIAL_VELOCITY{100.f, -350.f};
        const float BALL_RADIUS{12.5f};
        
        std::shared_ptr<LegacyShader> m_SpriteShader;
        std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
        std::unique_ptr<Texture> m_BackgroundTexture;
        std::shared_ptr<Texture> m_SolidBlockTexture;

        std::unique_ptr<BreakoutGameObject> m_Player;
        std::shared_ptr<Texture> m_PaddleTexture;

        std::unique_ptr<Ball> m_Ball;
        std::shared_ptr<Texture> m_FaceTexture;

        std::unique_ptr<ParticleGenerator> m_Particles;
        std::shared_ptr<LegacyShader> m_ParticleShader;
        std::shared_ptr<Texture> m_ParticleTexture;

        irrklang::ISoundEngine* m_SoundEngine;
        
        bool CheckCollision(const BreakoutGameObject& one, const BreakoutGameObject& two) const; //AABB - AABB collision
        Collision CheckCollision(const Ball& one, const BreakoutGameObject& two) const; //AABB - Circle collision
        EDirection GetClosestDirection(glm::vec2 direction) const;
        void ResetLevel();
        void ResetPlayer();
        glm::vec2 GetPlayerStartPosition();
        glm::vec2 GetBallStartPosition(const glm::vec2& playerPosition);
    };
}

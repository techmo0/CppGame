#include "Game.h"

#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "Ghost.h"

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Game::Game(const glm::ivec2& windowSize)
    : m_eCurrentGameState(EGameState::Active)
    , m_windowSize(windowSize)
{
    m_keys.fill(false);
}

Game::~Game()
{

}

void Game::render()
{
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();

    if (m_pGhost)
    {
        m_pGhost->render();
    }
}

void Game::update(const uint64_t delta)
{
    ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);

    if (m_pGhost)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pGhost->setOrientation(Ghost::EOrientation::Top);
            m_pGhost->move(true);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_pGhost->setOrientation(Ghost::EOrientation::Left);
            m_pGhost->move(true);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_pGhost->setOrientation(Ghost::EOrientation::Right);
            m_pGhost->move(true);
        }
        else if (m_keys[GLFW_KEY_S])
        {
            m_pGhost->setOrientation(Ghost::EOrientation::Bottom);
            m_pGhost->move(true);
        }
        else
        {
            m_pGhost->move(false);
        }

        m_pGhost->update(delta);
    }
}

void Game::setKey(const int key, const int action)
{
    m_keys[key] = action;
}

bool Game::init()
{
    auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
        return false;
    }

    auto tex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_16x16.png");

    std::vector<std::string> subTexturesNames = {
        "block",
        "topBlock",
        "bottomBlock",
        "leftBlock",
        "rightBlock",
        "topLeftBlock",
        "topRightBlock",
        "bottomLeftBlock",

        "bottomRightBlock",
        "beton",
        "topBeton",
        "bottomBeton",
        "leftBeton",
        "rightBeton",
        "topLeftBeton",
        "topRightBeton",

        "bottomLeftBeton",
        "bottomRightBeton",
        "water1",
        "water2",
        "water3",
        "trees",
        "ice",
        "wall",

        "eagle",
        "deadEagle",
        "nothing",
        "respawn1",
        "respawn2",
        "respawn3",
        "respawn4"
    };

    auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTexturesNames), 16, 16);

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");
    pAnimatedSprite->setPosition(glm::vec2(300, 300));
    std::vector<std::pair<std::string, uint64_t>> waterState;
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1000000000));
    waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1000000000));

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

    pAnimatedSprite->insertState("waterState", std::move(waterState));
    pAnimatedSprite->insertState("eagleState", std::move(eagleState));

    pAnimatedSprite->setState("waterState");

    pDefaultShaderProgram->use();
    pDefaultShaderProgram->setInt("tex", 0);

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

    pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);


    std::vector<std::string> GhostSubTexturesNames = {
        "GhostFront1",
        "GhostFront2",
        "GhostBack1",
        "GhostBack2",
        "GhostLeft1",
        "GhostLeft2",
        "GhostRight1",
        "GhostRight2"
    };
    auto pGhostTextureAtlas = ResourceManager::loadTextureAtlas("GhostTextureAtlas", "res/textures/Ghost.png", std::move(GhostSubTexturesNames), 16, 16);
    auto pGhostAnimatedSprite = ResourceManager::loadAnimatedSprite("GhostAnimatedSprite", "GhostTextureAtlas", "SpriteShader", 100, 100, "GhostTop1");

    std::vector<std::pair<std::string, uint64_t>> GhostTopState;
    GhostTopState.emplace_back(std::make_pair<std::string, uint64_t>("GhostBack1", 500000000));
    GhostTopState.emplace_back(std::make_pair<std::string, uint64_t>("GhostBack2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> GhostBottomState;
    GhostBottomState.emplace_back(std::make_pair<std::string, uint64_t>("GhostFront1", 500000000));
    GhostBottomState.emplace_back(std::make_pair<std::string, uint64_t>("GhostFront2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> GhostRightState;
    GhostRightState.emplace_back(std::make_pair<std::string, uint64_t>("GhostRight1", 500000000));
    GhostRightState.emplace_back(std::make_pair<std::string, uint64_t>("GhostRight2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> GhostLeftState;
    GhostLeftState.emplace_back(std::make_pair<std::string, uint64_t>("GhostLeft1", 500000000));
    GhostLeftState.emplace_back(std::make_pair<std::string, uint64_t>("GhostLeft2", 500000000));

    pGhostAnimatedSprite->insertState("GhostTopState",    std::move(GhostTopState));
    pGhostAnimatedSprite->insertState("GhostBottomState", std::move(GhostBottomState));
    pGhostAnimatedSprite->insertState("GhostLeftState",   std::move(GhostLeftState));
    pGhostAnimatedSprite->insertState("GhostRightState",  std::move(GhostRightState));

    pGhostAnimatedSprite->setState("GhostTopState");

    m_pGhost = std::make_unique<Ghost>(pGhostAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));
    return true;
}
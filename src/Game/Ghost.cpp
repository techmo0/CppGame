#include "Ghost.h"
#include "../Renderer/AnimatedSprite.h"

Ghost::Ghost(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position)
    : m_eOrientation(EOrientation::Top)
    , m_pSprite(std::move(pSprite))
    , m_move(false)
    , m_velocity(velocity)
    , m_position(position)
    , m_moveOffset(glm::vec2(0.f, 1.f))
{
    m_pSprite->setPosition(m_position);
}

void Ghost::render() const
{
    m_pSprite->render();
}

void Ghost::setOrientation(const EOrientation eOrientation)
{
    if (m_eOrientation == eOrientation)
    {
        return;
    }

    m_eOrientation = eOrientation;
    switch (m_eOrientation)
    {
    case Ghost::EOrientation::Top:
        m_pSprite->setState("GhostTopState");
        m_moveOffset.x = 0.f;
        m_moveOffset.y = 1.f;
        break;
    case Ghost::EOrientation::Bottom:
        m_pSprite->setState("GhostBottomState");
        m_moveOffset.x = 0.f;
        m_moveOffset.y = -1.f;
        break;
    case Ghost::EOrientation::Left:
        m_pSprite->setState("GhostLeftState");
        m_moveOffset.x = -1.f;
        m_moveOffset.y = 0.f;
        break;
    case Ghost::EOrientation::Right:
        m_pSprite->setState("GhostRightState");
        m_moveOffset.x = 1.f;
        m_moveOffset.y = 0.f;
        break;
    default:
        break;
    }
}

void Ghost::move(const bool move)
{
    m_move = move;
}

void Ghost::update(const uint64_t delta)
{
    if (m_move)
    {
        m_position += delta * m_velocity * m_moveOffset;
        m_pSprite->setPosition(m_position);
        m_pSprite->update(delta);
    }
}

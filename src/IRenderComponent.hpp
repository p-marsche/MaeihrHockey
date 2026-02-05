#pragma once

#include "IComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace mmt_gd
{
class IRenderComponent : public IComponent
{
public:
    using Ptr     = std::shared_ptr<IRenderComponent>;
    using WeakPtr = std::weak_ptr<IRenderComponent>;

    IRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow) :
    IComponent(gameObject),
    m_renderWindow(renderWindow),
    m_isVisible(true)
    {
    }

    virtual void draw() = 0;

    [[nodiscard]] bool isVisible() const
    {
        return m_isVisible;
    }

    void setVisibility(bool visible)
    {
        m_isVisible = visible;
    }

protected:
    sf::RenderWindow& m_renderWindow;
    bool              m_isVisible;
};
} // namespace mmt_gd

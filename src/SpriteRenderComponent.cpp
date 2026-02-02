#include "stdafx.h"

#include "SpriteRenderComponent.hpp"
#include "AssetManager.hpp"
#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"

#include <utility>

namespace mmt_gd
{
SpriteRenderComponent::SpriteRenderComponent(GameObject&       gameObject,
                                             sf::RenderWindow& renderWindow,
                                             std::string       textureName,
                                             std::string       layerName,
                                             sf::IntRect       textureRect) :
IRenderComponent(gameObject, renderWindow),
m_textureName(textureName),
m_layerName(std::move(layerName)),
m_textureRect(textureRect),
m_hasTextureRect(textureRect.width > 0 && textureRect.height > 0)
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layerName, *this));
}

SpriteRenderComponent::~SpriteRenderComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
}

bool SpriteRenderComponent::init()
{
    m_sprite.setTexture(AssetManager::getInstance().getTexture(m_textureName));

    // Apply texture rect AFTER setTexture (which resets to full texture)
    if (m_hasTextureRect)
    {
        m_sprite.setTextureRect(m_textureRect);
    }

    return true;
}

void SpriteRenderComponent::draw()
{
    // TODO: adapt transform to have pixel precise movement

    m_renderWindow.draw(m_sprite, m_gameObject.getTransform());
}
} // namespace mmt_gd

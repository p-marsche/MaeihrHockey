//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#include "stdafx.h"

#include "SpriteRenderComponent.hpp"

#include "AssetManager.hpp"
#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"

#include <utility>

namespace mmt_gd
{
SpriteRenderComponent::SpriteRenderComponent(
    GameObject&       gameObject,
    sf::RenderWindow& renderWindow,
    sf::Texture&       texture,
    std::string       layerName,
    sf::IntRect       textureRect) :
IRenderComponent(gameObject, renderWindow),
m_texture(texture),
m_layerName(std::move(layerName)),
m_textureRect(textureRect),
m_hasTextureRect(textureRect.width > 0 && textureRect.height > 0),
m_state()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layerName, *this));
}

SpriteRenderComponent::~SpriteRenderComponent()
{
    EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
}

bool SpriteRenderComponent::init()
{
    m_sprite.setTexture(m_texture);

    // Apply texture rect AFTER setTexture (which resets to full texture)
    if (m_hasTextureRect)
    {
        m_sprite.setTextureRect(m_textureRect);
    }

    return true;
}

void SpriteRenderComponent::registerShaderFuncs(const ShaderFunction& func)
{
    m_shaderFuncs.push_back(func);
}

void SpriteRenderComponent::applyShaderFuncs(sf::RenderStates& state)
{
    for (const auto& f : m_shaderFuncs)
    {
        f(state);
    }
}

void SpriteRenderComponent::draw()
{
    // TODO: adapt transform to have pixel precise movement
    m_state = m_gameObject.getTransform();

    applyShaderFuncs(m_state);
    
    m_renderWindow.draw(m_sprite, m_state);
}
} // namespace mmt_gd

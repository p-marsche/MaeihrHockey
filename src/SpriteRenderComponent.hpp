//This code was made for the Multimedia Project 2a for the Multimedia Technology class at Fachhochschule Salzburg, by Pascal Veselka & Pascal Marsche
#pragma once

#include "IRenderComponent.hpp"

#include <SFML/Graphics.hpp>
#include <string>

namespace mmt_gd
{
class SpriteRenderComponent final : public IRenderComponent
{
public:
    using Ptr = std::shared_ptr<SpriteRenderComponent>;
    using ShaderFunction = std::function<void(sf::RenderStates&)>;

    SpriteRenderComponent(GameObject&       gameObject,
                          sf::RenderWindow& renderWindow,
                          sf::Texture&      texture,
                          std::string       layerName,
                          sf::IntRect       textureRect = sf::IntRect());

    ~SpriteRenderComponent() override;

    bool init() override;

    void update(float deltaTime) override
    {
    }

    void registerShaderFuncs(const ShaderFunction& func);
    void applyShaderFuncs(sf::RenderStates& state);

    void draw() override;

    sf::Sprite& getSprite()
    {
        return m_sprite;
    }

    void setTextureRect(const sf::IntRect& rect)
    {
        m_textureRect    = rect;
        m_hasTextureRect = true;
    }

    void setScale(const sf::Vector2f& scale)
    {
        m_sprite.setScale(scale);
    }

    void setScale(float scaleX, float scaleY)
    {
        m_sprite.setScale(scaleX, scaleY);
    }

private:
    sf::Texture& m_texture;
    sf::RenderStates m_state;
    sf::Sprite   m_sprite;
    std::string  m_layerName;
    sf::IntRect  m_textureRect;
    bool         m_hasTextureRect = false;
    std::list<ShaderFunction> m_shaderFuncs;
};
} // namespace mmt_gd

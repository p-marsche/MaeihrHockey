#include "stdafx.h"

#include "AssetManager.hpp"

namespace mmt_gd
{
AssetNotFoundException::AssetNotFoundException()
{
    m_text = "";
}
AssetNotFoundException::AssetNotFoundException(std::string message) : m_text(message)
{
}

const char* AssetNotFoundException::what() const noexcept
{
    return m_text.c_str();
}


AssetManager& AssetManager::getInstance()
{
    static AssetManager assetManager;
    return assetManager;
}

void AssetManager::loadTexture(std::string name, std::string filename)
{
    auto ret = m_textures.try_emplace(name, std::make_unique<sf::Texture>());
    if (ret.second)
        m_textures[name]->loadFromFile(Config::texturesPath + filename);
}

void AssetManager::loadSoundBuffer(std::string name, std::string filename)
{
    auto ret = m_soundBuffers.try_emplace(name, std::make_unique<sf::SoundBuffer>());
    if (ret.second)
        m_soundBuffers[name]->loadFromFile(Config::soundsPath + filename);
}

void AssetManager::loadFont(std::string name, std::string filename)
{
    auto ret = m_fonts.try_emplace(name, std::make_unique<sf::Font>());
    if (ret.second)
        m_fonts[name]->loadFromFile(Config::fontsPath + filename);
}

void AssetManager::loadMusic(std::string name, std::string filename)
{
    auto ret = m_music.try_emplace(name, std::make_unique<sf::Music>());
    if (ret.second)
        m_music[name]->openFromFile(Config::musicPath + filename);
}

void AssetManager::loadImage(std::string name, std::string filename)
{
    auto ret = m_images.try_emplace(name, std::make_unique<sf::Image>());
    if (ret.second)
        m_images[name]->loadFromFile(Config::imagesPath + filename);
}

void AssetManager::replaceTexture(std::string name, std::string filename)
{
    if (auto it = m_textures.find(name) != m_textures.end())
    {
        auto extracted = m_textures.extract(name);
        auto ptr       = extracted.mapped().release();
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
    m_textures.emplace(name, std::make_unique<sf::Texture>());
    m_textures[name]->loadFromFile(Config::texturesPath + filename);
}

void AssetManager::replaceSoundBuffer(std::string name, std::string filename)
{
    if (auto it = m_soundBuffers.find(name) != m_soundBuffers.end())
    {
        auto extracted = m_soundBuffers.extract(name);
        auto ptr       = extracted.mapped().release();
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
    m_soundBuffers.emplace(name, std::make_unique<sf::SoundBuffer>());
    m_soundBuffers[name]->loadFromFile(Config::soundsPath + filename);
}

void AssetManager::replaceFont(std::string name, std::string filename)
{
    if (auto it = m_fonts.find(name) != m_fonts.end())
    {
        auto extracted = m_fonts.extract(name);
        auto ptr       = extracted.mapped().release();
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
    m_fonts.emplace(name, std::make_unique<sf::Font>());
    m_fonts[name]->loadFromFile(Config::fontsPath + filename);
}

void AssetManager::replaceMusic(std::string name, std::string filename)
{
    if (auto it = m_music.find(name) != m_music.end())
    {
        auto extracted = m_music.extract(name);
        auto ptr       = extracted.mapped().release();
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
    m_music.emplace(name, std::make_unique<sf::Music>());
    m_music[name]->openFromFile(Config::musicPath + filename);
}

void AssetManager::replaceImage(std::string name, std::string filename)
{
    if (auto it = m_images.find(name) != m_images.end())
    {
        auto extracted = m_images.extract(name);
        auto ptr       = extracted.mapped().release();
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }
    m_images.emplace(name, std::make_unique<sf::Image>());
    m_images[name]->loadFromFile(Config::imagesPath + filename);
}

sf::Texture& AssetManager::getTexture(std::string name)
{
    if (m_textures.find(name) != m_textures.end())
        return *m_textures[name];
    else
        throw new AssetNotFoundException("The texture '{name}' could not be found!");
}

sf::SoundBuffer& AssetManager::getSoundBuffer(std::string name)
{
    if (m_soundBuffers.find(name) != m_soundBuffers.end())
        return *m_soundBuffers[name];
    else
        throw new AssetNotFoundException("The sound buffer '{name}' could not be found!");
}

sf::Font& AssetManager::getFont(std::string name)
{
    if (m_fonts.find(name) != m_fonts.end())
        return *m_fonts[name];
    else
        throw new AssetNotFoundException("The font '{name}' could not be found!");
}

sf::Music& AssetManager::getMusic(std::string name)
{
    if (m_music.find(name) != m_music.end())
        return *m_music[name];
    else
        throw new AssetNotFoundException("The music file '{name}' could not be found!");
}

sf::Image& AssetManager::getImage(std::string name)
{
    if (m_images.find(name) != m_images.end())
        return *m_images[name];
    else
        throw new AssetNotFoundException("The image file '{name}' could not be found!");
}

AssetManager::~AssetManager()
{
    m_textures.clear();
    m_music.clear();
    m_soundBuffers.clear();
    m_fonts.clear();
}
} // namespace mmt_gd
#include "stdafx.h"

#include "WindowHandler.hpp"

void WindowHandler::init(std::string title, int width, int height, int initialScaling)
{
    m_title        = title;
    m_width        = width;
    m_height       = height;
    m_scale        = initialScaling;
    m_isFullscreen = true;

    sf::VideoMode mode = sf::VideoMode((width * m_scale), (height * m_scale));
    createWindow(mode, sf::Style::Fullscreen);
}

void WindowHandler::toggleFullscreen()
{
    if (!m_isFullscreen)
        createWindow(sf::VideoMode::getDesktopMode(), sf::Style::Fullscreen);
    else
    {
        sf::VideoMode mode = sf::VideoMode((m_width * m_scale), (m_height * m_scale));
        createWindow(mode, sf::Style::Default);
    }
    m_isFullscreen = !m_isFullscreen;
}

void WindowHandler::resizeWindow()
{
    sf::Vector2u newSize = m_window.getSize();
    if (m_window.getSize().x >= sf::VideoMode::getDesktopMode().width) // when maximising window
        newSize.x = (m_window.getSize().y * m_width / m_height);       // take window height as base and calculate width
    else
    {
        newSize.y = (m_window.getSize().x * m_height / m_width); // otherwise window width is base
        if (m_window.getSize().y < newSize.y) // prevent scaling of height if smaller than calculated height
            m_window.setSize(sf::Vector2u(m_window.getSize().x, newSize.y));
    }
    float    viewportWidth  = (float)newSize.x / m_window.getSize().x;
    float    viewportHeight = (float)newSize.y / m_window.getSize().y;
    sf::View view           = m_window.getView();
    view.setViewport(sf::FloatRect((1 - viewportWidth) / 2, (1 - viewportHeight) / 2, viewportWidth, viewportHeight));
    m_window.setView(view);
}

void WindowHandler::createWindow(sf::VideoMode mode, int style)
{
    m_window.create(mode, m_title, style);
    //Image icon = AssetManager.Instance.GetImage("Window Icon");
    //m_window.SetIcon(icon.Size.X, icon.Size.Y, icon.Pixels);
    sf::View view = sf::View(sf::Vector2f(m_width / 2, m_height / 2), sf::Vector2f(m_width, m_height));
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    m_window.setView(view);

    m_window.setMouseCursorVisible(false);
    m_window.setFramerateLimit(120);
}

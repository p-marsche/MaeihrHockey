#pragma once

class WindowHandler
{
public:
    void init(std::string title, int width, int height, tgui::Gui* gui, int initialScaling = 1);
    void toggleFullscreen();
    void resizeWindow();

    sf::RenderWindow m_window;

private:
    void createWindow(sf::VideoMode mode, int style);

    bool        m_isFullscreen;
    int         m_scale;
    std::string m_title;
    int         m_width;
    int         m_height;

    tgui::Gui* m_gui;
};
#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <fstream>

class Game
{
private:
    const int width = sf::VideoMode::getDesktopMode().width;
    const int height = sf::VideoMode::getDesktopMode().height;
    sf::Color color;
    std::vector<std::string> words;
public:
    Game()  {
        color = sf::Color(47,79,79, 255);

    };
    ~Game(){};
    bool Setup();
    void lifeCycle();
    void menuCycle(std::vector<sf::Text> escTexts, sf::RenderWindow window);
    void gameCycle();
    sf::Vector2f getCenter()
    {
        sf::Vector2f center((float)width/2, (float)height/2);
        return center;
    }
};
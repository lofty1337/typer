#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <cmath>
class Game
{
private:
    const int width = sf::VideoMode::getDesktopMode().width;
    const int height = sf::VideoMode::getDesktopMode().height;
    sf::Color bgColor;
    sf::Font font;
    std::ifstream text;
    std::vector<std::string> words;
    sf::RectangleShape field;
    sf::Text input;
    sf::Text stream;
    sf::RectangleShape underline;
    sf::Vector2f underlineDefSize;
    sf::Text scoreText;
    sf::Text timer;
    std::string inStr;
    std::string streamStr;
    std::vector<sf::Text> escTexts;
    int score;
    int wordCount;
    int escCount;
    int inChSize;
    int chSize;
    int streamChSize;
    char typed;
    sf::Clock clock;
    sf::RenderWindow window;
public:
    Game();
    ~Game(){};
    bool Setup();
    void lifeCycle();
    sf::Vector2f getCenter()
    {
        sf::Vector2f center((float)width/2, (float)height/2);
        return center;
    }
    void shuffleStream();
    void startGame();
    void escMenu();
    void eventLoop(sf::Event event);
    void gameplay();
    bool isMenuOpened();
};
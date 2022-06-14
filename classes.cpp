#include "header.h"

void Game::lifeCycle() {
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cout << "can't load font" << std::endl;
    }
    std::vector<sf::Text> escTexts;
    escTexts.push_back(sf::Text("Start", font));
    escTexts.push_back(sf::Text("Quit", font));
    int chSize = 100;
    for (int i = 0; i < escTexts.size(); i++) {
        escTexts[i].setCharacterSize(chSize);
        escTexts[i].setFillColor(sf::Color::White);
        escTexts[i].setOrigin(0, 0);
        escTexts[i].setPosition(getCenter().x - escTexts[i].getGlobalBounds().width / 2,
                                getCenter().y + chSize * i - escTexts[i].getGlobalBounds().height * escTexts.size());
        //std::cout << escTexts[i].getGlobalBounds().width << ' ' << escTexts[i].getGlobalBounds().height << '\n';
    }

    int escCount = 0;
    sf::RenderWindow window(sf::VideoMode(), "typer", sf::Style::Fullscreen);

    while (window.isOpen()) {
        window.clear(color);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    escCount++;

        }

        if (escCount % 2 == 0)
        {
            for (int i = 0; i < escTexts.size(); i++) {
                window.draw(escTexts[i]);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    if (sf::Mouse::getPosition().x >= escTexts[i].getPosition().x)
                        if (sf::Mouse::getPosition().x <=
                            escTexts[i].getPosition().x + escTexts[i].getGlobalBounds().width)
                            if (sf::Mouse::getPosition().y >= escTexts[i].getPosition().y)
                                if (sf::Mouse::getPosition().y <=
                                    escTexts[i].getPosition().y + escTexts[i].getGlobalBounds().height)
                                    if(escTexts[i].getString()=="Quit")
                                        window.close();
                                    else if(escTexts[i].getString()=="Start")
                                      ++escCount;
            }
        }
        else//actual gameplay
        {

        }
        window.display();
    }
}


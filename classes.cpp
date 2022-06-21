#include "header.h"
#define SPACE_KEY 32
#define BACKSPACE_KEY 8
#define ESCAPE_KEY 27
Game::Game(){
    bgColor = sf::Color(47,79,79, 255);
    font.loadFromFile("fonts/arial.ttf");
    std::ifstream text("texts/eng.txt");

    chSize = 100;
    streamChSize = 50;

    std::string word;
    while (text >> word)
        words.push_back(word);

    stream.setFillColor(sf::Color::White);
    stream.setFont(font);
    stream.setCharacterSize(streamChSize);
    shuffleStream();

    field.setSize(sf::Vector2f(400,50));
    field.setFillColor(sf::Color::White);
    field.setOutlineColor(sf::Color::Blue);
    field.setOutlineThickness(5);
    field.setPosition(getCenter().x-field.getSize().x/2, stream.getPosition().y - 100);

    input.setFont(font);
    input.setFillColor(sf::Color::Black);
    input.setCharacterSize(30);
    input.setPosition(sf::Vector2f(field.getPosition().x + 5,
                                   field.getPosition().y + 5));
    score = 0;
    wordCount = 0;

    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setCharacterSize(50);
    scoreText.setPosition(sf::VideoMode::getDesktopMode().width-250,0);


    escTexts.push_back(sf::Text("WPM:"+std::to_string(score), font));
    escTexts.push_back(sf::Text("Start", font));
    escTexts.push_back(sf::Text("Quit", font));
    for (int i = 0; i < escTexts.size(); i++) {
        escTexts[i].setCharacterSize(chSize);
        escTexts[i].setOrigin(sf::Vector2f(0,0));
        escTexts[i].setFillColor(sf::Color::White);
        escTexts[i].setPosition(getCenter().x - escTexts[i].getGlobalBounds().width / 2,
                                getCenter().y + chSize * i - escTexts[i].getGlobalBounds().height * escTexts.size());
    }

    timer.setFillColor(sf::Color::White);
    timer.setFont(font);
    timer.setCharacterSize(chSize);

    escCount = 0;
}
void Game::lifeCycle() {
    window.create(sf::VideoMode(), "typer", sf::Style::Fullscreen);
    while (window.isOpen()) {
        window.clear(bgColor);
        sf::Event event;
        while (window.pollEvent(event)) {
            eventLoop(event);
        }
        if (escCount % 2 == 0)
            escMenu();
        else
            gameplay();
        if (clock.getElapsedTime().asSeconds() >= 60) {
            escTexts[0].setString("WPM: "+std::to_string(score));
            clock.restart();
            escCount++;
        }
        window.display();
        }
    }

void Game::shuffleStream() {
    wordCount=0;
    streamStr.clear();
    std::default_random_engine rng(time(0));
    std::shuffle(std::begin(words), std::end(words), rng);
    for(int i = 0;i<10;i++)
        streamStr+=(words[i]+' ') ;
    stream.setString(streamStr);
    stream.setPosition(getCenter().x - stream.getGlobalBounds().width / 2,
                       getCenter().y - stream.getGlobalBounds().height);
    input.setPosition(sf::Vector2f(field.getPosition().x + 5,
                                   field.getPosition().y + 5));
}
void Game::escMenu(){
    for (int i = 0; i < escTexts.size(); i++)
        if(!((i==0)&&(escCount<1)))//{
            window.draw(escTexts[i]);
            //if (sf::Mouse::isButtonPressed(sf::Mouse::Left))

        //}
}
void Game::eventLoop(sf::Event event) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                escTexts[0].setString("WPM: " + std::to_string(score));
                startGame();
                inStr.clear();
            }
            break;
        case sf::Event::TextEntered:
            if ((event.text.unicode < 128)
                && (event.text.unicode != BACKSPACE_KEY)
                && (event.text.unicode != SPACE_KEY)
                   && (event.text.unicode != ESCAPE_KEY)) {
                typed = static_cast<char>(event.text.unicode);
                inStr += typed;
            }
            if (event.text.unicode == BACKSPACE_KEY)
                if (!inStr.empty())
                    inStr.pop_back();
            if (event.text.unicode == SPACE_KEY) {
                if (inStr == words[wordCount])
                    score++;
                scoreText.setString("Score:" + std::to_string(score));
                if(!inStr.empty())
                    wordCount++;
                if (wordCount == 10)
                    shuffleStream();
                inStr.clear();
            }
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
                for (int i = 0; i < escTexts.size(); i++){
                    if (sf::Mouse::getPosition().x >= escTexts[i].getPosition().x)
                        if (sf::Mouse::getPosition().x <=
                            escTexts[i].getPosition().x + escTexts[i].getGlobalBounds().width)
                            if (sf::Mouse::getPosition().y >= escTexts[i].getPosition().y)
                                if (sf::Mouse::getPosition().y <=
                                    escTexts[i].getPosition().y + escTexts[i].getGlobalBounds().height)
                                    if (escTexts[i].getString() == "Quit")
                                        window.close();
                                    else if (escTexts[i].getString() == "Start")
                                        startGame();
                }
    }
}
void Game::gameplay() {
    input.setString(inStr);
    window.draw(scoreText);
    window.draw(field);
    window.draw(input);
    window.draw(timer);
    window.draw(stream);
    std::string timerStr = std::to_string((int) clock.getElapsedTime().asSeconds());
    timer.setString(timerStr);
}
void Game::startGame() {
    inStr.clear();
    ++escCount;
    shuffleStream();
    clock.restart();
}
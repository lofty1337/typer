#include "header.h"
#define SPACE_KEY 32
#define BACKSPACE_KEY 8
#define ESCAPE_KEY 27
#define TAB_KEY 9
#define WPM_STR "WPM: "
#define START_STR "Start (tab)"
#define QUIT_STR "Quit (esc)"
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

    inChSize = 30;
    input.setFont(font);
    input.setFillColor(sf::Color::Black);
    input.setCharacterSize(inChSize);
    input.setPosition(sf::Vector2f(field.getPosition().x + 5,
                                   field.getPosition().y + 5));
    score = 0;
    wordCount = 0;

    underlineDefSize=sf::Vector2f(0,3);
    underline.setFillColor(sf::Color::Red);
    underline.setSize(underlineDefSize);
    underline.setPosition(sf::Vector2f(stream.getPosition()));

    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setCharacterSize(50);
    scoreText.setPosition(sf::VideoMode::getDesktopMode().width-250,0);

    float height=0;
    escTexts.push_back(sf::Text(WPM_STR+std::to_string(score), font));
    escTexts.push_back(sf::Text(START_STR, font));
    escTexts.push_back(sf::Text(QUIT_STR, font));
    for (int i = 0; i < escTexts.size(); i++) {
        escTexts[i].setCharacterSize(chSize);
        height+=escTexts[i].getGlobalBounds().height;
        escTexts[i].setOrigin(sf::Vector2f(0,0));
        escTexts[i].setFillColor(sf::Color::White);
    }
    for(int i =0;i<escTexts.size();i++)
        escTexts[i].setPosition(getCenter().x - escTexts[i].getGlobalBounds().width / 2,
                                getCenter().y -height/2-escTexts[escTexts.size()-i-1].getGlobalBounds().height*(escTexts.size()-i-1));

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
        if (isMenuOpened())
            escMenu();
        else
            gameplay();
        if (clock.getElapsedTime().asSeconds() >= 60) {
            escTexts[0].setString(WPM_STR+std::to_string(score));
            clock.restart();
            escCount++;
        }
        window.display();
        }
    }

void Game::shuffleStream() {
    underline.setSize(underlineDefSize);
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
    underline.setPosition(sf::Vector2f(stream.getPosition().x,
                                       stream.getPosition().y+stream.getGlobalBounds().height+7));
}
void Game::escMenu(){
    for (int i = 0; i < escTexts.size(); i++)
        if(!((i==0)&&(escCount<1))) //doesn't show WPM if you haven't played yet
            window.draw(escTexts[i]);
}
void Game::eventLoop(sf::Event event) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                if(isMenuOpened())
                    window.close();
                else {
                    escTexts[0].setString(WPM_STR + std::to_string(score));
                    escTexts[0].setPosition(getCenter().x - escTexts[0].getGlobalBounds().width / 2,
                                            getCenter().y -height/2-
                                            escTexts[escTexts.size()-1].getGlobalBounds().height*(escTexts.size()-1));
                    startGame();
                    inStr.clear();
                }
            }
            if (event.key.code == sf::Keyboard::Tab)
                if(isMenuOpened())
                    startGame();
            break;
        case sf::Event::TextEntered:
            if ((event.text.unicode < 128)
                && (event.text.unicode != BACKSPACE_KEY)
                && (event.text.unicode != SPACE_KEY)
                   && (event.text.unicode != ESCAPE_KEY)
                      && (event.text.unicode != TAB_KEY)) {
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
                if(!inStr.empty()) {
                    sf::Text tmp;
                    tmp.setCharacterSize(streamChSize);
                    tmp.setString(words[wordCount]+" ");
                    tmp.setFont(font);
                    int addSize = tmp.getGlobalBounds().width;
                    underline.setSize(sf::Vector2f(underline.getSize().x+addSize,underlineDefSize.y));
                    wordCount++;
                }
                if (wordCount == 10)
                    shuffleStream();
                inStr.clear();
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
    window.draw(underline);
    std::string timerStr = std::to_string((int)clock.getElapsedTime().asSeconds());
    timer.setString(timerStr);
}
void Game::startGame() {
    score=0;
    scoreText.setString("Score:" + std::to_string(score));
    inStr.clear();
    ++escCount;
    shuffleStream();
    clock.restart();
}
bool Game::isMenuOpened() {
    if (escCount%2==0)
        return true;
    else
        return false;
}
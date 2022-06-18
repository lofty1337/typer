#include "header.h"

void Game::lifeCycle() {
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
        std::cout << "can't load font\n";
    std::ifstream text("texts/eng.txt");
    if (!text.is_open())
        std::cout << "can't load text\n";

    int chSize = 100;
    int streamChSize = 50;
    char typed;
    char lastTyped;

    std::vector<std::string> words;
    std::string word;
    while (text >> word)
        words.push_back(word);


    sf::Text stream;
    stream.setFillColor(sf::Color::White);
    stream.setFont(font);
    stream.setCharacterSize(streamChSize);
    std::string streamStr;



    sf::RectangleShape field(sf::Vector2f(400, 50));
    field.setFillColor(sf::Color::White);
    field.setOutlineColor(sf::Color::Blue);
    field.setOutlineThickness(5);

    sf::Text input;
    input.setFont(font);
    input.setFillColor(sf::Color::Black);
    input.setCharacterSize(30);
    std::string inStr;

    int score = 0;
    int wordCount = 0;
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setCharacterSize(50);
    scoreText.setPosition(sf::VideoMode::getDesktopMode().width-200,0);

    std::vector<sf::Text> escTexts;
    escTexts.push_back(sf::Text("WPM:"+std::to_string(score), font));
    escTexts.push_back(sf::Text("Start", font));
    escTexts.push_back(sf::Text("Quit", font));
    for (int i = 0; i < escTexts.size(); i++) {
        escTexts[i].setCharacterSize(chSize);
        escTexts[i].setFillColor(sf::Color::White);
        escTexts[i].setPosition(getCenter().x - escTexts[i].getGlobalBounds().width / 2,
                                getCenter().y + chSize * i - escTexts[i].getGlobalBounds().height * escTexts.size());
    }

    int escCount = 0;
    sf::RenderWindow window(sf::VideoMode(), "typer", sf::Style::Fullscreen);

    sf::Clock delay;
    sf::Clock clock;
    sf::Text timer;
    timer.setFillColor(sf::Color::White);
    timer.setFont(font);
    timer.setCharacterSize(chSize);

    while (window.isOpen()) {
        window.clear(color);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape){
                    escTexts[0].setString("WPM: "+std::to_string(score));
                    escCount++;
                }
        }

        if (escCount % 2 == 0) {
            for (int i = 0; i < escTexts.size(); i++)
                if(!((i==0)&&(escCount<1)))
                {
                    window.draw(escTexts[i]);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                        if (sf::Mouse::getPosition().x >= escTexts[i].getPosition().x)
                            if (sf::Mouse::getPosition().x <=
                                escTexts[i].getPosition().x + escTexts[i].getGlobalBounds().width)
                                if (sf::Mouse::getPosition().y >= escTexts[i].getPosition().y)
                                    if (sf::Mouse::getPosition().y <=
                                        escTexts[i].getPosition().y + escTexts[i].getGlobalBounds().height)
                                        if (escTexts[i].getString() == "Quit")
                                            window.close();
                                        else if (escTexts[i].getString() == "Start") {
                                            ++escCount;
                                            wordCount=0;
                                            streamStr.clear();
                                            std::random_device rd;
                                            auto rng = std::default_random_engine { rd() };
                                            std::shuffle(std::begin(words), std::end(words), rng);
                                            for(int i = 0;i<10;i++)
                                                streamStr+=(words[i]+' ') ;
                                            stream.setString(streamStr);
                                            stream.setPosition(getCenter().x - stream.getGlobalBounds().width / 2,
                                                               getCenter().y - stream.getGlobalBounds().height);
                                            field.setPosition(stream.getPosition().x, stream.getPosition().y - 100);
                                            input.setPosition(sf::Vector2f(field.getPosition().x + 5,
                                                                           field.getPosition().y + 5));
                                            clock.restart();
                                            delay.restart();
                                        }
                }
        }
        else//gameplay
        {
            input.setString(inStr);
            window.draw(scoreText);
            window.draw(field);
            window.draw(input);
            window.draw(timer);
            window.draw(stream);
            std::string timerStr = std::to_string((int) clock.getElapsedTime().asSeconds());
            timer.setString(timerStr);
            if (event.type == sf::Event::TextEntered) {
                if ((event.text.unicode < 128)
                && (event.text.unicode != 8)
                && (event.text.unicode != 32)) {
                    if (delay.getElapsedTime().asMilliseconds() <= 325) {
                        typed = static_cast<char>(event.text.unicode);
                        if (lastTyped != typed)
                            inStr += typed;
                        lastTyped = typed;
                    }
                    else {
                        delay.restart();
                        typed = static_cast<char>(event.text.unicode);
                        inStr += typed;
                        lastTyped = typed;
                    }
                }
                if (event.text.unicode == 8)
                    if (delay.getElapsedTime().asMilliseconds() >=100)
                        if (!inStr.empty()) {
                            delay.restart();
                            inStr.pop_back();
                        }
                if (event.text.unicode == 32)
                    if (delay.getElapsedTime().asMilliseconds() >=100){
                        delay.restart();
                        if(inStr==words[wordCount])
                            score++;
                        scoreText.setString("Score:"+std::to_string(score));
                        wordCount++;
                        if(wordCount>10)
                        {
                            wordCount = 0;
                            streamStr.clear();
                            std::random_device rd;
                            auto rng = std::default_random_engine { rd() };
                            std::shuffle(std::begin(words), std::end(words), rng);
                            for(int i = 0;i<10;i++)
                                streamStr+=(words[i]+' ') ;
                            stream.setString(streamStr);
                            stream.setPosition(getCenter().x - stream.getGlobalBounds().width / 2,
                                               getCenter().y - stream.getGlobalBounds().height);
                            field.setPosition(stream.getPosition().x, stream.getPosition().y - 100);
                            input.setPosition(sf::Vector2f(field.getPosition().x + 5,
                                                           field.getPosition().y + 5));
                        }
                        inStr.clear();
                    }
            }
        }

            if (clock.getElapsedTime().asSeconds() >= 60) {
                escTexts[0].setString("WPM: "+std::to_string(score));
                clock.restart();
                escCount++;
            }

            window.display();
        }
    }

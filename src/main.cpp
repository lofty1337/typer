#include "header.h"
int main()
{
    sf::Font font;
    std::ifstream text("texts/eng.txt");
    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cout << "can't load font\n";
        return -1;
    }
    if (!text.is_open()){
        std::cout << "can't load text\n";
        return -1;
    }
    Game game;
    game.lifeCycle();
    return 0;
}
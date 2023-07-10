#include <SFML/Graphics.hpp>
#include "Board.h"

using namespace std;


int main()
{
    Board mineSweeper;
    sf::RenderWindow window(sf::VideoMode(mineSweeper.getWidth(), mineSweeper.getHeight()), "Patrick's Minesweeper");
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
        window.clear();
        mineSweeper.play(window, event);
        window.display();
    }

    return 0;
}

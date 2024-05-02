#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Pacman.h"

using namespace std;

const int GRID_WIDTH = 1100;
const int GRID_HEIGHT = 900;
const int GRID_SIZE = 50;
const int NUM_ROWS = GRID_HEIGHT / GRID_SIZE;
const int NUM_COLS = GRID_WIDTH / GRID_SIZE;

int main()
{
    
    sf :: Texture text;
    text.loadFromFile("./Sprites/Maze.jpg");
    sf :: Sprite background;
    background.setTexture(text);
    background.setPosition(GRID_SIZE,GRID_SIZE);
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "PacmanOS");
    float timer = 0;
    sf :: Clock clock;
    Pacman pacman;

    while (window.isOpen())
    {
        window.clear();
        float time = clock.getElapsedTime().asSeconds();
        pacman.timer += time;
        clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
        }
        sf::Color cellColor = sf::Color::White;

        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < NUM_COLS; ++j) {
                sf::RectangleShape cell(sf::Vector2f(GRID_SIZE - 1, GRID_SIZE - 1));
                cell.setFillColor(sf::Color::Black); 
                cell.setOutlineThickness(1);  
                cell.setOutlineColor(sf::Color::White);
                cell.setPosition( j * GRID_SIZE + GRID_SIZE,i * GRID_SIZE + GRID_SIZE);
                window.draw(cell);
            }
        }

        if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::A))
            pacman.move('l');    
        if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::D)) 
            pacman.move('r');  
        if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::W))
            pacman.move('u');    
        if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::S))
            pacman.move('d');
        window.draw(background);
        pacman.draw(window);
        window.display();
    }

    return 0;
}
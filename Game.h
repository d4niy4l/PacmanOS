#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Pacman.h"
#include "Maze.h"
using namespace std;

class Game{
    
private:
public:
    void start_game(){
        sf::RenderWindow window(sf::VideoMode(1000, 1000), "PacmanOS");
        sf :: Clock clock;
        Pacman pacman(25 + 150, 25 + 100);
        Maze maze("dungeon","./Sprites/Tile.png","./Sprites/Platform.png");
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
            if(maze.descended){
                if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::A))
                    pacman.move('l',maze);    
                else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::D)) 
                    pacman.move('r',maze);  
                else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::W))
                    pacman.move('u',maze);    
                else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::S))
                    pacman.move('d',maze);
            }
            maze.draw(window);
            pacman.draw(window);
            window.display();
        }
    }

};

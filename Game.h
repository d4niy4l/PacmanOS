#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Pacman.h"
#include "DungeonMaze.h"
using namespace std;

class Game{
    
private:
public:
    void start_game(){
        sf :: Texture tile_texts;
        tile_texts.loadFromFile("./Sprites/Tile.png");
        sf::RenderWindow window(sf::VideoMode(1000, 1000), "PacmanOS");
        sf :: Clock clock;
        Pacman pacman(175, 125);
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

            if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::A))
                pacman.move('l');    
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::D)) 
                pacman.move('r');  
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::W))
                pacman.move('u');    
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::S))
                pacman.move('d');
            for(int i = 0;i<dungeon.size();i++){
                for(int j = 0;j<dungeon[i].size();j++){\
                    if(dungeon[i][j] == 0){
                        sf :: Sprite tile;
                        tile.setTexture(tile_texts);
                        tile.setPosition(j*25 + 150,i*25 + 100 );
                        window.draw(tile);
                    }
                }
            }
            pacman.draw(window);
            window.display();
        }
    }

};

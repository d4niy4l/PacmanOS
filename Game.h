#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Pacman.h"

using namespace std;

class Game{
    vector<vector<int>> grid;
private:
public:
    
    void start_game(){
        sf :: Texture text;
        sf :: Sprite background;
        text.loadFromFile("./Sprites/Maze.png");
        background.setTexture(text);
        background.setPosition(300,100);
        sf::RenderWindow window(sf::VideoMode(1000, 800), "PacmanOS");
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

            if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::A))
                pacman.move('l');    
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::D)) 
                pacman.move('r');  
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::W))
                pacman.move('u');    
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::S))
                pacman.move('d');
            window.draw(background);
            pacman.draw(window);
            window.display();
        }
    }

    void map_grid(string path){
        sf :: Image maze;
        maze.loadFromFile(path);

        int width = maze.getSize().x;
        int height = maze.getSize().y;
    
        for(int i = 0;i<width;i++){
            grid.push_back({});
            for(int j = 0;j<height;j++){
                sf::Color pixel = maze.getPixel(i,j);
                if(pixel.toInteger() == 8452351){
                    grid[i].push_back(0);
                }
                else if(pixel.toInteger() == 255){
                    grid[i].push_back(1);
                }

            }
        }
        for(int i = 0;i<height;i++){
            for(int j = 0;j<width;j++){
                cout<<grid[j][i];
            }
            cout<<endl;
        }
    }
};

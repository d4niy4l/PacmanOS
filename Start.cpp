#pragma once
#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Pacman.h"
#include "Ghost.h"
#include "Maze.h"
#include <vector>
#include "pthread.h"

using namespace std;
pthread_mutex_t gameOverMutex;

Pacman pacman;
Maze maze;
array<Ghost,4> ghosts;
array<pthread_t,4> ghostThreads;
bool gameOver = false;


void start_game(){
    //  INITIALIZING MUTEXES
    sf::RenderWindow window(sf::VideoMode(1000, 800), "PacmanOS");
    sf :: Clock clock;
    char pressed_dir;
    while (window.isOpen())
    {
        window.clear();
        sf::Time eTime = clock.getElapsedTime();
        float time = eTime.asSeconds();
        
        pacman.timer += time;
        for(int i = 0;i<ghosts.size();i++){
            ghosts[i].timer += time;
        }
        clock.restart();
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
        }
        if(maze.descended){
            if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::A))
                pressed_dir = 'l';   
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::D)) 
                pressed_dir = 'r';
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::W))
                pressed_dir = 'u';
            else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::S))
                pressed_dir = 'd';
        }
        maze.draw(window);
        if(maze.descended == true){
            pacman.draw(window);
            for(int i = 0;i<ghosts.size();i++){
                ghosts[i].draw(window);
            }
            
        }
        pacman.move(pressed_dir,maze, time);
        window.display();
    }
}

int main(){
    ghosts[0].initialize("Blinky");
    ghosts[1].initialize("Pinky");
    ghosts[2].initialize("Inky");
    ghosts[3].initialize("Clyde");    
    start_game();
    return 0;
}
#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Pacman.h"
#include "Ghost.h"
#include "Maze.h"
#include <vector>
#include "pthread.h"
#include "Globals.h"
#include "GhostFunctions.h"
using namespace std;



//THREAD FUNCTION PROTOTYPES
void* manageGhosts(void* singleGhostArgs);

//UI THREAD
void* start_game(void*){
    background_text.loadFromFile("./Sprites/background.jpg");
    background.setTexture(background_text);
    background.setPosition(0, 37);
    hudFont.loadFromFile("./Fonts/Title_Font.ttf");
    score.setFont(hudFont);
    score.setPosition(450,10);
    score.setCharacterSize(30);
    score.setString("SCORE");
    while(true){
        if(timer > 0.01){
            pacman.move(pressed_dir,maze,timer);
            timer = 0;
        }
    }
    pthread_exit(NULL);
}


void* manageGhosts(void* singleGhostArgs) {
  Ghost* g = (Ghost*)singleGhostArgs;
  sf::Clock c;
  float elapsed_time = 0;
  while (!gameOver) {
    //pthread_mutex_lock(&gameOverMutex);
    if (appeared == true && elapsed_time > 20){
      float time = c.getElapsedTime().asSeconds();
      moveGhost(*g, time);
      elapsed_time -= 20;
    }
    if (maze.descended == false) {
      elapsed_time = c.restart().asMilliseconds();
    }else{
      elapsed_time += c.restart().asMilliseconds();
    }
    //pthread_mutex_unlock(&gameOverMutex);

  }
  return 0;
}
//MAIN LOOP THREAD
int main(){
    ghosts[0].initialize("Blinky");
    ghosts[1].initialize("Pinky");
    ghosts[2].initialize("Inky");
    ghosts[3].initialize("Clyde");    
    pacman.sprite.setScale(0,0);
    for(int i = 0;i<4;i++){
        ghosts[i].sprite.setScale(0,0);
    }
    for (int i = 0; i < ghostThreads.size(); i++) {
        if (pthread_create(&ghostThreads[i], NULL, manageGhosts ,&ghosts[i]) != 0) {
            perror("ERROR: Unable to create threads.\n");
            return 1;
        }
    }
    pthread_attr_t gameThread_attr;
    pthread_attr_init(&gameThread_attr);
    pthread_attr_setdetachstate(&gameThread_attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&gameThread, &gameThread_attr,start_game,nullptr);
    pthread_attr_destroy(&gameThread_attr);

    //INITIALIZE SFML ELEMENTS
    sf :: Clock clock;
    sf::Event event;
    window.create(sf::VideoMode(900, 800), "Hollow Pac");

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
                
            
        }
        window.clear();
        sf::Time eTime = clock.getElapsedTime();
        float time = eTime.asSeconds();
        maze.timer += time;
        pacman.timer += time;
        if(maze.descended == false){
            maze.laser.timer += time;
        }
        for(int i = 0;i<ghosts.size();i++){
            ghosts[i].timer += time;
        }
        timer += time;
        clock.restart();
        window.draw(background);
        maze.draw(window,time);
        
        if(maze.descended){
            pacman.draw(window);
            for(int i = 0;i<ghosts.size();i++){
                ghosts[i].draw(window);
            }
            if(appeared){
                if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::A))
                    pressed_dir = 'l';   
                else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::D)) 
                    pressed_dir = 'r';
                else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::W))
                    pressed_dir = 'u';
                else if (sf :: Keyboard::isKeyPressed(sf :: Keyboard::S))
                    pressed_dir = 'd';
            }
        }
        if(maze.descended == true  && appeared == false){
            bool completed = true;
            sf :: Color color = pacman.sprite.getColor();
            
            if(pacman.sprite.getScale().x < 1){
                pacman.sprite.setScale(pacman.sprite.getScale().x + 0.01,pacman.sprite.getScale().y + 0.01);
                completed = false;
            }
            for(int i = 0;i<4;i++){
                if(ghosts[i].sprite.getScale().x < 1){
                    ghosts[i].sprite.setScale(ghosts[i].sprite.getScale().x + 0.01,ghosts[i].sprite.getScale().y + 0.01);
                }
            }
            if(completed == true){
                appeared = true;
            }
        }
        //window.draw(score);
        window.display();
    }
    gameOver = true;
    for (int i = 0; i < ghostThreads.size(); i++) {
        if (pthread_join(ghostThreads[i], NULL) != 0) {
            perror("ERROR: Unable to join thread.\n");
            return 2;
        }
    }   

    return 0;
}
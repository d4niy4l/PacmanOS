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
    hudFont.loadFromFile("./Fonts/HUD_Font.ttf");
    score.setFont(hudFont);
    score.setPosition(375,50);
    score.setCharacterSize(40);
    score.setString("SCORE");
    score.setScale(0,0);
    sf :: Color color = sf :: Color(81,79,79,255);
    score.setFillColor(color);
    score_int.setFont(hudFont);
    score_int.setPosition(520,50);
    score_int.setCharacterSize(40);
    score_int.setFillColor(color);
    score_int.setString("0");
    score_int.setScale(0,0);
    while(true){
        
        if(pactimer > 0.014){
            pacman.move(pressed_dir,maze,1);
            pactimer = 0;
            int gridRow = int((pacman.y) / 25);
            int gridCol = int((pacman.x) / 25);
            if (maze[gridRow][gridCol] == 2){
                pacman.score += 1;
                maze[gridRow][gridCol] = 1;
                score_int.setString(to_string(pacman.score));
            }
        }
        if(maze.descended == true  && appeared == false){
            if(score.getScale().x < 1){
                score.setScale(score.getScale().x + 0.05, score.getScale().y + 0.05);
            }
            if(score_int.getScale().x < 1){
                score_int.setScale(score_int.getScale().x + 0.05, score_int.getScale().y + 0.05);
            }
        }
    }
    pthread_exit(NULL);
}


void* manageGhosts(void* singleGhostArgs) {
    Ghost* g = (Ghost*)singleGhostArgs;
    srand(time(0));
    while (gameOver == false) {
        //pthread_mutex_lock(&gameOverMutex);
        if (appeared == true && ghosttimers[g->id] > 0.014){
            moveGhost(*g);
            ghosttimers[g->id] = 0;
            int dice = rand() % 10 + 1;
            pair<int,int> target = get_target(*g);
            int g_row = target.first;
            int g_col = target.second;
            if(g->chaseTimer > 10 && dice == 6 && g->chaseMode == true){
                g->chaseMode = !g->chaseMode;
                g->chaseTimer = 0;
            }
            else if(g->chaseTimer > 6 && dice == 6 && g->chaseMode == true){
                g->chaseMode = !g->chaseMode;
                g->chaseTimer = 0;
            }
            else if((g_row == (int)g->y/25 && g_col == (int) g->x/25) && g->chaseMode == false){
                g->chaseMode = true;
            }
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
        ghosttimers[i] = 0;
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
    window.setFramerateLimit(60);
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
        pactimer += time;
        if(maze.descended == false){
            maze.laser.timer += time;
        }
        for(int i = 0;i<ghosts.size();i++){
            ghosts[i].timer += time;
            ghosttimers[i] += time;
            ghosts[i].chaseTimer += time;
        }
        timer += time;
        clock.restart();
        window.draw(background);
        maze.draw(window,time);
        
        if(maze.descended){
            for(int i = 0;i<ghosts.size();i++){
                ghosts[i].draw(window);
            }
            pacman.draw(window);
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
                pacman.sprite.setScale(pacman.sprite.getScale().x + 0.02,pacman.sprite.getScale().y + 0.02);
                completed = false;
            }
            for(int i = 0;i<4;i++){
                if(ghosts[i].sprite.getScale().x < 1){
                    ghosts[i].sprite.setScale(ghosts[i].sprite.getScale().x + 0.02,ghosts[i].sprite.getScale().y + 0.02);
                }
            }
            if(completed == true){
                appeared = true;
            }
        }
        window.draw(score);
        window.draw(score_int);
        window.display();
    }
    gameOver = true;
    for (int i = 0; i < ghostThreads.size(); i++) {
        if (pthread_join(ghostThreads[i], NULL) != 0) {
            perror("ERROR: Unable to join thread.\n");
            exit(EXIT_FAILURE);
        }
    }   

    return 0;
}
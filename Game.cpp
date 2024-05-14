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

//PRODUCER CONSUMER THREAD TIMERS
int producer_timer = 0;
int consumer_timer = 0;
float pellet_timer = 0;
//THREAD FUNCTION PROTOTYPES
void* manageGhosts(void* singleGhostArgs);

//UI THREAD
void* ui_thread(void*){
    noLife.loadFromFile("./Sprites/NoLife.png");
    Life.loadFromFile("./Sprites/Life.png");
    for(int i = 0;i<3;i++){
        lives[i].setTexture(Life);
        lives[i].setPosition(maze_offset_x + i * 80, 740);
    }
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

    for (int i = 0; i < 2; i++) {
        keys[i].setSize(sf::Vector2f(25,25));
        keys[i].setFillColor(sf::Color::Red);
    }
    keys[0].setPosition(9 * 25+  maze_offset_x, 10*25 + maze_offset_y );
    keys[1].setPosition(13 * 25+  maze_offset_x, 10*25 + maze_offset_y );


    for (int i = 0; i < 2; i++) {
        exitPermits[i].setSize(sf::Vector2f(25,25));
        exitPermits[i].setFillColor(sf::Color::Green);
    }
    exitPermits[0].setPosition(9 * 25+  maze_offset_x, 12*25 + maze_offset_y );
    exitPermits[1].setPosition(13 * 25+  maze_offset_x, 12*25 + maze_offset_y );


    while(true){
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

void* pelletProducer(void*){
    sf :: Clock clock;
    srand(time(0));
    vector<pair<int, int>> cords = {{1, 1}, {1, maze[0].size()-2}, {maze.size()-2, 1}, {maze.size()-2, maze[0].size()-2}};

    while(true){
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
       // if(producer_timer > 0.014){
            producer_timer = 0;
            pellet_timer += time;
            int index = rand() % 4;
            if(maze[cords[index].first][cords[index].second] == 3){
                continue;
            }
            if(pellet_timer > PELLETPRODUCEINTERVAL){
                sem_wait(&space);
                sem_wait(&mutex);
                int row = cords[index].first;
                int col = cords[index].second;
                pellet_timer = 0;
                pthread_mutex_lock(&checkPellet);
                maze[row][col] = 3;
                pthread_mutex_unlock(&checkPellet);
                cout<<"spawned\n";
                sem_post(&mutex);
                sem_post(&full);
            }
        }
   // }
}
void* pelletConsumer(void*){
    vector<pair<int, int>> cords = {{1, 1}, {1, maze[0].size()-2}, {maze.size()-2, 1}, {maze.size()-2, maze[0].size()-2}};
    while(true){
            pthread_mutex_lock(&readincMutex);
            readcount += 1;
            if(readcount == 1){
                pthread_mutex_lock(&readWriteCords);
            }
            pthread_mutex_unlock(&readincMutex);
            int px = pacman.x/25;
            int py = pacman.y/25;
            pthread_mutex_lock(&readincMutex);
            readcount -= 1;
            if(readcount == 0){
                pthread_mutex_unlock(&readWriteCords);
            }
            pthread_mutex_unlock(&readincMutex);
            pthread_mutex_lock(&checkPellet);
            if(maze[py][px] == 3 && pacman.super == false){
                sem_wait(&full);
                sem_wait(&mutex);
                pacman.super = true;
                pacAte = true;
                pellet_timer = 0;
                if(px == 1 && py == maze.size() - 2){
                    lightening.setAnimationPosition(25 + maze_offset_x, -25);
                }
                else if (px == 1 && py == 1){
                    lightening.setAnimationPosition(25 + maze_offset_x, -1 * ((maze.size() - 2) * 25));
                }
                else if(px == maze[0].size() - 2 && py == 1){
                    lightening.setAnimationPosition(25 + maze_offset_x + (maze[0].size() - 3) * 25, -1 * ((maze.size() - 2) * 25));
                }
                else if (px == maze[0].size() - 2 && py == maze.size() - 2){
                    lightening.setAnimationPosition(25 + maze_offset_x + (maze[0].size() - 3) * 25, -25);
                }
                for(int i = 0;i<4;i++){
                    ghosts[i].isScared = true;
                    ghosts[i].toggle_sprite();
                    ghosts[i].timer = 10;
                }
                maze[py][px] = 1;
                sem_post(&mutex);
                sem_post(&space);
            }
            pthread_mutex_unlock(&checkPellet);
    }
}

void* manageGhosts(void* singleGhostArgs) {
    Ghost* g = (Ghost*)singleGhostArgs;
    srand(time(0));
    while (gameOver == false) {
        //pthread_mutex_lock(&gameOverMutex);
        if (appeared == true && ghosttimers[g->id] > 0.014 && hit == false){
            
            if (g->hasEscaped == true) {
                moveGhost(*g);
                ghosttimers[g->id] = 0;
                if(g->isScared == false){
                    int dice = rand() % 10 + 1;
    
                    if(g->chaseTimer > 30 && dice == 5 && g->chaseMode == true){
                        g->chaseMode = !g->chaseMode;
                        g->chaseTimer = 0;
                    }
                    else if(g->chaseTimer > 5 && g->chaseMode == false){
                        g->chaseMode = !g->chaseMode;
                        g->chaseTimer = 0;
                    }
                }
                if(g->isScared == true && g->scared_timer <= 0){
                    g->scared_timer = 0;
                    g->isScared = false;
                    g->toggle_sprite();
                    g->chaseMode = true;
                    g->chaseTimer = 0;
                    pacman.super = false;
                }
                pthread_mutex_lock(&checkCollision);
                int px = pacman.x/25;
                int py = pacman.y/25;
                int gx = g->x/25;
                int gy = g->y/25;
                if(abs(pacman.x - g->x) < 20 && abs(pacman.y-g->y) < 20 &&  g->isScared == true){
                    g->isEaten = true;
                    g->isScared = false;
                    pacman.score += 25;
                    score_int.setString(to_string(pacman.score));
                    g->toggle_sprite();
                }
                else if(abs(pacman.x - g->x) < 20 && abs(pacman.y-g->y) < 20 && hit == false&&  g->isEaten == false && pacman.lives > 0){
                    hit = true;
                    pacman.sprite.setTexture(pacman.death_texture);
                    pacman.sprite.setTextureRect(sf :: IntRect(0, 0, pacman.sprite.getTexture()->getSize().x / 8.0, pacman.sprite.getTexture()->getSize().y));
                    pacman.curr_frame_x = 0;
                    pacman.lives--;
                    lives[pacman.lives].setTexture(noLife);            
                }
                // else if(gx == px && gy == py && hit == false && g->isEaten == false && pacman.lives > 0){
                                
                // }
                pthread_mutex_unlock(&checkCollision);
            }else{
                
                //  2 KEYS => (0,1) WILL BE PICKED BASED ON GHOST'S ID
                int keyIdx = g->id % 2;

                //  WAIT FOR KEY
                if (sem_wait(&key[keyIdx]) !=  0) {
                    cout << "Ghost " << g->name << "got unexpected error waiting for key.\n";
                }
                
                cout << "Ghost " << g->name <<" has acquired the key " << keyIdx << endl;
                
                int y = (keys[keyIdx].getPosition().y - maze_offset_y) /25;
                int x = (keys[keyIdx].getPosition().x - maze_offset_x) /25;
                //  cout << g->name << " " << x << " " << y << endl;
                int gx = g->y/25;
                int gy = g->x/25;
                //  GO AND COLLECT THE KEY
                int collision = 0;
                float kx = keys[keyIdx].getPosition().x - maze_offset_x;
                float ky = keys[keyIdx].getPosition().y - maze_offset_y;
                while (!collision) {
                    if (ghosttimers[g->id] > 0.014) {
                        ghosttimers[g->id] = 0;
                        string dir = getShortestPath(gx,gy,y,x); 
                        
                        checkAndMove(*g, dir[0]);
                        if (abs(kx - g->x) < 25 && abs(ky-g->y) < 20) {
                            collision++;
                            cout << "Ghost "<< g->name << " got the key. \n";
                        }
                        gx = g->y/25;
                        gy = g->x/25;
                    }
                }
                

                //  NOW WAIT FOR PERMIT
                if (sem_wait(&exitPermit[keyIdx]) !=  0) {
                    cout << "Ghost " << g->name << "got unexpected error waiting for exit permit.\n";
                }
                
                cout << "Ghost " << g->name <<" has acquired the exit permit" << keyIdx << endl;
                
                int permitIdx = keyIdx;
                y = (exitPermits[permitIdx].getPosition().y - maze_offset_y) /25;
                x = (exitPermits[permitIdx].getPosition().x - maze_offset_x) /25;
                gx = g->y/25;
                gy = g->x/25;
                collision = 0;
                kx = exitPermits[permitIdx].getPosition().x - maze_offset_x;
                ky = exitPermits[permitIdx].getPosition().y - maze_offset_y;
                //cout << g->name << " Permit " <<  " " << x << " " << y << endl;
                while (!collision) {
                    if (ghosttimers[g->id] > 0.014) {
                        ghosttimers[g->id] = 0;
                        string dir = getShortestPath(gx,gy,y,x); 
                        checkAndMove(*g, dir[0]);
                        if (abs(kx - g->x) < 20 && abs(ky-g->y) < 20) {
                            collision++;
                            cout << "Ghost "<< g->name << " got the permit. \n";
                        }
                        gx = g->y/25;
                        gy = g->x/25;
                    }
                }
                
                
                g->hasEscaped = true;
                cout << "Ghost " << g->name << " has escaped.\n";
                //  RELEASE PERMIT
                if (sem_post(&exitPermit[keyIdx]) !=  0) {
                    cout << "Ghost " << g->name << "got unexpected error posting for exit permit.\n";
                }
                //  RELEASE KEY
                if (sem_post(&key[keyIdx]) !=  0) {
                    cout << "Ghost " << g->name << "got unexpected error posting for key.\n";
                }
            }
        }
   
    }
    return 0;
}

void reset_entities(){
    pacman.x = pacman.spawn_x*25;
    pacman.y =  pacman.spawn_y*25;
    pressed_dir = ' ';
    pacman.sprite.setTexture(pacman.right);
    pacman.sprite.setTextureRect(sf :: IntRect(0,0,pacman.right.getSize().x/2.0, pacman.right.getSize().y));
    pacman.sprite.setPosition(pacman.x + maze_offset_x, pacman.y + maze_offset_y);
    for(int i = 0;i<4;i++){
        ghosts[i].x = ghosts[i].spawn_col * 25;
        ghosts[i].y = ghosts[i].spawn_row * 25;
        ghosts[i].sprite.setPosition(ghosts[i].x + maze_offset_x, ghosts[i].y + maze_offset_y);
        ghosts[i].isScared = false;
        ghosts[i].scared_timer = 10;
        ghosts[i].hasEscaped = false;
        ghosts[i].chaseMode = false;
        ghosts[i].chaseTimer = 0;
        ghosts[i].sprite.setTexture(ghosts[i].text);
    }
    if(pacman.lives == 0){
        pacman.lives = 3;
        pacman.score = 0;
        for(int i = 0;i<3;i++){
            lives[i].setTexture(Life);
        }
        maze.repopulate_maze();
    }
    deathFinished = false;
    hit = false;
    
}

void* pacmanthread(void*){
    while(true){
    if(pactimer > 0.014){
            pacman.move(pressed_dir,maze,1);
            pactimer = 0;
            if(hit == false){
            int px = pacman.x/25;
            int py = pacman.y/25;
            pthread_mutex_lock(&checkCollision);
            if (maze[py][px] == 2){
                    pacman.score += 1;
                    maze[py][px] = 1;
                    score_int.setString(to_string(pacman.score));
                }
            pthread_mutex_unlock(&checkCollision);
            
            }
        }
    }
}

//MAIN LOOP THREAD
int main(){

    sem_init(&space,0,0);
    sem_init(&full, 0, 4);
    sem_init(&mutex, 0, 1);

    scared.loadFromFile("./Sprites/Scared.png");
    eaten_texture.loadFromFile("./Sprites/Eaten.png");
    
    srand(time(0));
    
    sem_init(&key[0], 0, 1);
    sem_init(&key[1], 0, 1);
    sem_init(&exitPermit[0], 0, 1);
    sem_init(&exitPermit[1], 0, 1);
    
    ghosts[0].initialize("Blinky");
    ghosts[1].initialize("Pinky");
    ghosts[2].initialize("Inky");
    ghosts[3].initialize("Clyde");    
    pacman.sprite.setScale(0,0);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    for(int i = 0;i<4;i++){
        ghosts[i].sprite.setScale(0,0);
        ghosttimers[i] = 0;
    }
    for (int i = 0; i < ghostThreads.size(); i++) {
        if (pthread_create(&ghostThreads[i], &attr, manageGhosts ,&ghosts[i]) != 0) {
            perror("ERROR: Unable to create threads.\n");
            return 1;
        }
    }
    pthread_attr_t pacmanThread_attr;
    pthread_create(&gameThread, &attr,ui_thread,nullptr);
    pthread_create(&pacmanThread, &attr, pacmanthread, nullptr);
    pthread_create(&consumerThread,&attr, pelletConsumer, nullptr);
    pthread_create(&producerThread,&attr, pelletProducer, nullptr);
    pthread_attr_destroy(&attr);

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
        //if(maze.descended == true && appeared == true){
            producer_timer += time;
            consumer_timer += time;
        //}
        maze.timer += time;
        pacman.timer += time;
        pactimer += time;
        if(maze.descended == false){
            maze.laser.timer += time; 
            lightening.timer += time;
            lightening.upDateAnimation();
        }
        for(int i = 0;i<ghosts.size();i++){
            if(appeared){
                ghosts[i].timer += time;
                ghosttimers[i] += time;
                ghosts[i].chaseTimer += time;
                if(ghosts[i].isScared && ghosts[i].scared_timer >= 0){
                    ghosts[i].scared_timer -= time;
                }
                clyde_timer += time;
            }
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
            if(pacAte == true){
                window.draw(lightening.sprite);
            }
            if(pacAte == true && lightening.animationFinished()){
                lightening.restartAnimation();
                pacAte = false;
            }
            if(appeared){
                int px = pacman.x/25;
                int py = pacman.y/25;
                if(py == 11){
                    if(px == 0){
                        pacman.x = (maze[0].size() - 2)*25;
                        pacman.sprite.setPosition(pacman.x * 25 + maze_offset_x,pacman.y*25 + maze_offset_y);
                    }
                    else if(px == maze[0].size() - 2){
                        pacman.x = 25;
                        pacman.sprite.setPosition(pacman.x * 25 + maze_offset_x,pacman.y*25 + maze_offset_y);
                    }
                }
                for (int i = 0; i < 2; i++) {
                    window.draw(keys[i]);
                    window.draw(exitPermits[i]);
                }
                if(deathFinished == true){
                    reset_entities();
                }
                for(int i = 0;i<3;i++){
                    window.draw(lives[i]);
                }
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

    return 0;
}
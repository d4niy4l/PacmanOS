#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include "Maze.h"
using namespace std;

class Ghost{
    public:
    float x;
    float y;
    int id;
    sf :: Texture text;
    sf :: Sprite sprite;
    sf :: Texture scared;
    int curr_frame;
    float timer;
    bool isEaten;
    bool hasEscaped;
    bool isScared;
    float scared_timer;
    bool chaseMode;
    float speed;
    string name;
    char dir;
    float chaseTimer;
    Ghost(){} 
    
    void initialize(string name){
        this->name = name;
        string path = "./Sprites/" + name + ".png";
        text.loadFromFile(path);
        scared.loadFromFile("./Sprites/Scared.png");
        sprite.setTexture(text);
        sprite.setTextureRect(sf :: IntRect(25*4,0,25,25));
        curr_frame = 0;
        dir = 'u';
        chaseTimer = 0;
        chaseMode = true;
        hasEscaped = false;
        isEaten = false;
        isScared = false;
        scared_timer = 0;
        timer = 0;
        curr_frame = 4;
        chaseTimer = 0;
        if(name == "Blinky"){
            x = 25*11;
            y = 25*11;
            id = 0;
        }
        else if(name == "Pinky"){
            x = 25*10;
            y = 25*12;
            id = 1;
        }
        else if(name == "Inky"){
            x = 25*12;
            y = 25*12;
            id = 2;
        }
        else if(name == "Clyde"){
            x = 25*11;
            y = 25*12;
            id = 3;
        }
        speed = 1.8;
        sprite.setPosition(x + maze_offset_x, y + maze_offset_y);
    }

    void draw(sf :: RenderWindow& window){
        window.draw(sprite);
        flow_ghosts();
    }

  
  void toggle_sprite(){
        if(isScared == true){
            sprite.setTexture(scared);
            sprite.setTextureRect(sf :: IntRect(0,0,25,25));
            scared_timer = 10;
            curr_frame = 0;
        }
        else{
            sprite.setTexture(text);
            sprite.setTextureRect(sf :: IntRect(25*4,0,25,25));
            scared_timer = 0;
        }

    }
    private:

    void flow_ghosts(){
        int last_frame;
        if(isScared == true){
            last_frame = (scared_timer <= 2) ? 3 : 1;
            if(timer > 0.2){
                timer = 0;
                curr_frame++;
                if(curr_frame > last_frame){
                    curr_frame = 0;
                }
            }
            int left = curr_frame * 25;
            sprite.setTextureRect(sf :: IntRect(left, 0, 25,25));
            return;
        }
        if(dir == 'u'){
            last_frame = 5;
        }
        else if(dir == 'd'){
            last_frame = 7;
        }
        else if(dir == 'l'){
            last_frame = 3;
        }
        else if(dir == 'r'){
            last_frame = 1;
        }
        else{
            last_frame = rand() % 7 + 1;
        }
        if(timer > 0.2){
            timer = 0;
            curr_frame++;
            if(curr_frame > last_frame){
                curr_frame = last_frame - 1;
            }
        }
        int left = curr_frame * 25;
        sprite.setTextureRect(sf :: IntRect(left, 0, 25,25));
        
    }

};

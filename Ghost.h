#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Maze.h"
using namespace std;

class Ghost{
    public:
        float x;
        float y;
        sf :: Texture text;
        sf :: Sprite sprite;
        int curr_frame;
        float timer;
        bool isEaten;
        bool hasEscaped;
        bool chaseMode;
        float chaseTimer;
        string name;
        char dir;
        Ghost(){} 
      
        void initialize(string name){
            this->name = name;
            string path = "./Sprites/" + name + ".png";
            text.loadFromFile(path);
            sprite.setTexture(text);
            sprite.setTextureRect(sf :: IntRect(25*4,0,25,25));
            curr_frame = 0;
            dir = 'u';
            chaseTimer = 0;
            chaseMode = false;
            hasEscaped = false;
            isEaten = false;
            timer = 0;
            curr_frame = 4;
            if(name == "Blinky"){
                x = 25*11;
                y = 25*12;
            }
            else if(name == "Pinky"){
                x = 25*10;
                y = 25*12;
            }
            else if(name == "Inky"){
                x = 25*12;
                y = 25*12;
            }
            else if(name == "Clyde"){
                x = 25*11;
                y = 25*11;
            }
            sprite.setPosition(x + maze_offset_x, y + maze_offset_y);
        }

        void draw(sf :: RenderWindow& window){
            window.draw(sprite);
            flow_ghosts();
        }

        private:

        void flow_ghosts(){
            int last_frame;
             if(dir == 'u'){
                last_frame = 5;
            }
            else if(dir == 'd'){
                last_frame = 7;
            }
            else if(dir == 'l'){
                last_frame = 1;
            }
            else if(dir == 'r'){
                last_frame = 3;
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
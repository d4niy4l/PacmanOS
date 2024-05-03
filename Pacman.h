#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> 
#include "Maze.h"
class Pacman{
public:
    int x;
    int y;
    int left;
    int top;
    float timer;
    char dir;
    int curr_frame_x;
    int curr_frame_y;
    float speed;
    sf :: Texture text;
    sf :: Sprite sprite;
    Pacman(int x = 0, int y = 0){
        text.loadFromFile("./Sprites/PacMan.png");
        sprite.setTexture(text);
        sprite.setTextureRect(sf :: IntRect(0,0,text.getSize().x/2.0, text.getSize().y/4.0));
        left = top = 0;
        curr_frame_x = 0;
        curr_frame_y = 0;
        this->x = 1;
        this->y = 1;
        timer = 0;
        dir = 'l';
        sprite.setPosition(x,y);
        speed = 1;
    }
    void move(char pressed_dir, Maze& maze){
        switch (pressed_dir){
        case 'r':
            if(x+1 > maze[0].size() - 1){
                return;
            }
            else if(maze[y][x+1] == 0){
                return;
            }
            if((sprite.getPosition().x -150) > (x * 25)){
                x++;
            }
            move_mouth();
            sprite.move(speed,0);
            curr_frame_y = 0;
            break;
        case 'l':
            if(x-1 <= 0){
                return;
            }   
            if((sprite.getPosition().x -150) < (x * 25)){
                x--;
            }
            move_mouth();
            sprite.move(-1 * speed,0);
            curr_frame_y = 1;
            break;
        case 'u':
            if(y-1 < 0){
                return;
            }  
            if((sprite.getPosition().y -100) <= (y * 25)){
                y--;
            }
            move_mouth();
            curr_frame_y = 2;
            sprite.move(0,-1 * speed);
            break;
        case 'd':
            if(y + 1 > dungeon.size()-1){
                return;
            }  
            move_mouth();
            if((sprite.getPosition().y -100) >= (y * 25)){
                y++;
            }
            curr_frame_y = 3;
            sprite.move(0,speed);
            break;
        default:
            break;
        }

    }
    void move_mouth(){
        if(timer > 0.15){
            timer = 0;
            curr_frame_x += 1;
            if (curr_frame_x > 1){
                curr_frame_x = 0;
            }
        }
        left = (text.getSize().x / 2.0) * curr_frame_x;
        top = (text.getSize().y / 4.0) * curr_frame_y;
        sprite.setTextureRect(sf :: IntRect(left, top, text.getSize().x / 2.0, text.getSize().y / 4.0));
    }

    void draw(sf :: RenderWindow& window){
        window.draw(sprite);
    }
};
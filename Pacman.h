#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> 
#include "Maze.h"

enum{
    blinky = 0,
    pinky = 1,
    inky = 2,
    clyde = 3,
};
bool hit = false;
bool deathFinished = false;
class Pacman{
public:
    float x;
    float y;
    int left_val;
    float timer;
    char dir;
    int curr_frame_x;
    int curr_frame_y;
    int lives;
    float speed;
    int score;
    int spawn_x;
    int spawn_y;
    sf :: Texture up;
    sf :: Texture down;
    sf :: Texture left;
    sf :: Texture right;
    sf :: Texture death_texture;
    sf :: Sprite sprite;

    Pacman(){
        up.loadFromFile("./Sprites/up.png");
        down.loadFromFile("./Sprites/down.png");
        left.loadFromFile("./Sprites/left.png");
        right.loadFromFile("./Sprites/right.png");
        death_texture.loadFromFile("./Sprites/Death.png");
        sprite.setTexture(right);
        sprite.setTextureRect(sf :: IntRect(0,0,right.getSize().x/2.0, right.getSize().y));
        curr_frame_x = 0;
        curr_frame_y = 0;
        this->x = 25*11;
        this->y = 25*14;
        timer = 0;
        dir = ' ';
        spawn_x = 11;
        lives = 3;
        spawn_y = 14;
        sprite.setPosition(x + maze_offset_x, y + maze_offset_y);
        speed = 1.9;
        score = 0;
    }
    
    void move(char curr_dir, Maze& maze, float time){

        if(hit == true){
            move_mouth();
            return;
        }
        int gridRow = int((y) / 25);
        int gridCol = int((x) / 25);
        int gridX = gridCol * 25;
        int gridY = gridRow * 25;
        bool isRowAligned = abs(gridY - y) < 5;
        bool isColAligned = abs(gridX - x) < 5;
        bool canSwitch = can_change(curr_dir,isRowAligned,isColAligned,maze,gridRow,gridCol);
        if(canSwitch == false){
            curr_dir = dir;
        }
        switch (curr_dir){
        case 'r':
            if(!isRowAligned || (maze[gridRow][gridCol+1] == 0 || maze[gridRow][gridCol+1] == 4))
                break;
            sprite.setPosition(x + maze_offset_x, gridY + maze_offset_y);
            move_mouth();
            sprite.move(speed * time,0);
            x+= speed * time;
            sprite.setTexture(right);
         
            break;
        case 'l':
            if(!isRowAligned || (isColAligned && (maze[gridRow][gridCol - 1] == 0 || maze[gridRow][gridCol - 1] == 4)))
                break;
            sprite.setPosition(x + maze_offset_x, gridY + maze_offset_y);
            move_mouth();
            sprite.move(-1 * speed * time,0);
            x-=speed * time;
            curr_frame_y = 1;
            sprite.setTexture(left);
            break;
        case 'u':
            
            if(!isColAligned || (isRowAligned && (maze[gridRow - 1][gridCol] == 0 || maze[gridRow - 1][gridCol] == 4)))
                break;
            sprite.setPosition(gridX + maze_offset_x, y + maze_offset_y);
            move_mouth();
            curr_frame_y = 2;
            y -= speed * time;
            sprite.setTexture(up);
            sprite.move(0,-1 * speed * time);
            break;
        case 'd':
         
            if(!isColAligned|| (maze[gridRow + 1][gridCol] == 0 || maze[gridRow + 1][gridCol] == 4))
                break;
            sprite.setPosition(gridX + maze_offset_x,y + maze_offset_y );
            move_mouth();
            y += speed * time;
            curr_frame_y = 3;
            sprite.setTexture(down);
            sprite.move(0,speed * time);
            break;
        default:
            break;
        }
        dir = curr_dir ;
    }

    void draw(sf :: RenderWindow& window){
        window.draw(sprite);
    }

    
private:
    
    void move_mouth(){
        if(hit == true){
            if(timer > 0.10){
                timer = 0;
                curr_frame_x += 1;
                if(curr_frame_x > 7){
                    curr_frame_x = 0;
                    deathFinished = true;
                    dir = ' ';
                    return;
                }
            }
            int left_val = (sprite.getTexture()->getSize().x / 8.0) * curr_frame_x;
            sprite.setTextureRect(sf :: IntRect(left_val, 0, sprite.getTexture()->getSize().x / 8.0, sprite.getTexture()->getSize().y));
            return;
        }
        if(timer > 0.15){
            timer = 0;
            curr_frame_x += 1;
            if (curr_frame_x > 1){
                curr_frame_x = 0;
            }
        }
        left_val = (sprite.getTexture()->getSize().x / 2.0) * curr_frame_x;
        sprite.setTextureRect(sf :: IntRect(left_val, 0, sprite.getTexture()->getSize().x / 2.0, sprite.getTexture()->getSize().y));
    }
    
    bool can_change(char dir, bool rowAligned, bool colAligned, Maze& maze, int gridRow, int gridCol){
        if(dir == 'u'){
            return !(!colAligned || (rowAligned && (maze[gridRow - 1][gridCol] == 0 || maze[gridRow - 1][gridCol] == 4)));
        }
        else if(dir == 'd'){
            return !(!colAligned|| (maze[gridRow + 1][gridCol] == 0 || maze[gridRow + 1][gridCol] == 4));
        }
        else if(dir == 'l'){
            return !(!rowAligned || (colAligned && (maze[gridRow][gridCol - 1] == 0 || maze[gridRow][gridCol - 1] == 4)));
        }
        else if(dir == 'r'){
            return !(!rowAligned || (maze[gridRow][gridCol+1] == 0 || maze[gridRow][gridCol+1] == 4));
        }
        return true;
    }


    
};
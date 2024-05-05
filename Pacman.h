#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath> 
#include "Maze.h"
class Pacman{
public:
    float x;
    float y;
    int left;
    int top;
    float timer;
    char dir;
    int curr_frame_x;
    int curr_frame_y;
    float speed;
    sf :: Texture text;
    sf :: Sprite sprite;
    Pacman(){
        text.loadFromFile("./Sprites/PacMan.png");
        sprite.setTexture(text);
        sprite.setTextureRect(sf :: IntRect(0,0,text.getSize().x/2.0, text.getSize().y/4.0));
        left = top = 0;
        curr_frame_x = 0;
        curr_frame_y = 0;
        this->x = 25*11;
        this->y = 25*14;
        timer = 0;
        dir = ' ';
        sprite.setPosition(x + maze_offset_x, y + maze_offset_y);
        speed = 125;
    }
    
    void move(char curr_dir, Maze& maze, float time){
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
            if(!isRowAligned || (maze[gridRow][gridCol+1] == 0 || maze[gridRow][gridCol+1] == 2))
                break;
            sprite.setPosition(x + maze_offset_x, gridY + maze_offset_y);
            move_mouth();
            sprite.move(speed * time,0);
            x+= speed * time;
            curr_frame_y = 0;
         
            break;
        case 'l':
            if(!isRowAligned || (isColAligned && (maze[gridRow][gridCol - 1] == 0 || maze[gridRow][gridCol - 1] == 2)))
                break;
            sprite.setPosition(x + maze_offset_x, gridY + maze_offset_y);
            move_mouth();
            sprite.move(-1 * speed * time,0);
            x-=speed * time;
            curr_frame_y = 1;
            break;
        case 'u':
            
            if(!isColAligned || (isRowAligned && (maze[gridRow - 1][gridCol] == 0 || maze[gridRow - 1][gridCol] == 2)))
                break;
            sprite.setPosition(gridX + maze_offset_x, y + maze_offset_y);
            move_mouth();
            curr_frame_y = 2;
            y -= speed * time;
            sprite.move(0,-1 * speed * time);
            break;
        case 'd':
         
            if(!isColAligned|| (maze[gridRow + 1][gridCol] == 0 || maze[gridRow + 1][gridCol] == 2))
                break;
            sprite.setPosition(gridX + maze_offset_x,y + maze_offset_y );
            move_mouth();
            y += speed * time;
            curr_frame_y = 3;
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
    
    bool can_change(char dir, bool rowAligned, bool colAligned, Maze& maze, int gridRow, int gridCol){
        if(dir == 'u'){
            return !(!colAligned || (rowAligned && (maze[gridRow - 1][gridCol] == 0 || maze[gridRow - 1][gridCol] == 2)));
        }
        else if(dir == 'd'){
            return !(!colAligned|| (maze[gridRow + 1][gridCol] == 0 || maze[gridRow + 1][gridCol] == 2));
        }
        else if(dir == 'l'){
            return !(!rowAligned || (colAligned && (maze[gridRow][gridCol - 1] == 0 || maze[gridRow][gridCol - 1] == 2)));
        }
        else if(dir == 'r'){
            return !(!rowAligned || (maze[gridRow][gridCol+1] == 0 || maze[gridRow][gridCol+1] == 2));
        }
        return true;
    }


    
};
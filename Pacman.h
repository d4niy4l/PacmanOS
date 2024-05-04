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
        this->x = 25;
        this->y = 25;
        timer = 0;
        dir = 'l';
        sprite.setPosition(x,y);
        //sprite.setOrigin(sprite.getPosition().x/2,sprite.getPosition().y/2);
        speed = 1;
    }
    void move(char pressed_dir, Maze& maze){
        system("clear");
        cout<<"x: "<<x<<endl;
        cout<<"y: "<<y<<endl;
        int gridRow = int((y) / 25);
        int gridCol = int((x) / 25);
        int gridX = gridCol * 25;
        int gridY = gridRow * 25;
        bool isRowAligned = abs(gridY - y) < 5;
        bool isColAligned = abs(gridX - x) < 5;
        switch (pressed_dir){
        case 'r':
            isRowAligned = abs(gridY - y) < 5;
            isColAligned = abs(gridX - x) < 5;
            if(!isRowAligned || maze[gridRow][gridCol + 1] == 0)
                break;
            sprite.setPosition(x + 150, gridY + 100);
            move_mouth();
            sprite.move(speed,0);
            x+= speed;
            curr_frame_y = 0;
            break;
        case 'l':
            isRowAligned = abs(gridY - y) < 5;
            isColAligned = abs(gridX - x) < 5;
            if(!isRowAligned || (isColAligned && maze[gridRow][gridCol - 1] == 0))
                break;
            sprite.setPosition(x + 150, gridY + 100);
            move_mouth();
            sprite.move(-1 * speed,0);
            x-=speed;
            curr_frame_y = 1;
            break;
        case 'u':
            isRowAligned = abs(gridY - y) < 5;
            isColAligned = abs(gridX - x) < 5;
            if(!isColAligned || (isRowAligned && maze[gridRow - 1][gridCol] == 0))
                break;
            sprite.setPosition(gridX + 150, y + 100);
            move_mouth();
            curr_frame_y = 2;
            y -= speed;
            sprite.move(0,-1 * speed);
            break;
        case 'd':
            isRowAligned = abs(gridY - y) < 5;
            isColAligned = abs(gridX - x) < 5;
            if(!isColAligned|| maze[gridRow + 1][gridCol] == 0)
                break;
            sprite.setPosition(gridX + 150,y + 100 );
            move_mouth();
            y += speed;
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
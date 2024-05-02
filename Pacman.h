# pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

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
    int last_frame;
    sf :: Texture text;
    sf :: Sprite sprite;
    Pacman(){
        text.loadFromFile("./Sprites/PacMan.png");
        sprite.setTexture(text);
        sprite.setTextureRect(sf :: IntRect(0,0,text.getSize().x/2.0, text.getSize().y/4.0));
        left = top = 0;
        curr_frame_x = 0;
        curr_frame_y = 0;
        x = 0;
        y = 0;
        timer = 0;
        dir = 'l';
        sprite.setPosition(50,50);
    }
    void move(char pressed_dir){
        switch (pressed_dir){
        case 'r':
            move_mouth();
            x++;
            sprite.move(0.6,0);
            curr_frame_y = 0;
            break;
        case 'l':
            move_mouth();
            x--;
            sprite.move(-0.6,0);
            curr_frame_y = 1;
            break;
        case 'u':
            move_mouth();
            y--;
            curr_frame_y = 2;
            sprite.move(0,-0.6);
            break;
        case 'd':
            move_mouth();
            y++;
            curr_frame_y = 3;
            sprite.move(0,0.6);
            break;
        default:
            left = (text.getSize().x / 2.0);
            top = (text.getSize().y / 4.0) * curr_frame_y;
            sprite.setTextureRect(sf :: IntRect(left, top, text.getSize().x / 2.0, text.getSize().y / 4.0));
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
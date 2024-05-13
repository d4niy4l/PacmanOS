#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include "Maze.h"

Maze maze("dungeon","./Sprites/Tile.png","./Sprites/Platform.png");

using namespace std;
sf :: Texture scared;
sf :: Texture eaten_texture; 

class Ghost{
    public:
    float x;
    float y;
    int id;
    sf :: Texture text;
    sf :: Sprite sprite;
    int spawn_row;
    int spawn_col;
    int curr_frame;
    float timer;
    bool isEaten;
    bool hasEscaped;
    bool isScared;
    bool prev_hiding_point;
    float scared_timer;
    bool chaseMode;
    float speed;
    string name;
    char dir;
    float chaseTimer;
    queue<pair<int,int> >scatter_targets;
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
        chaseMode = !false;
        hasEscaped = false;
        isEaten = false;
        isScared = false;
        scared_timer = 0;
        timer = 0;
        curr_frame = 4;
        chaseTimer = 0;
        if(name == "Blinky"){
            spawn_col = 11;
            spawn_row = 11;
            id = 0;
            scatter_targets.push(pair<int,int>(1,1));
            scatter_targets.push(pair<int,int>(1,5));
            scatter_targets.push(pair<int,int>(3,5));
            scatter_targets.push(pair<int,int>(3,1));
        }
        else if(name == "Pinky"){
            spawn_col = 10;
            spawn_row = 12;
            id = 1;
            scatter_targets.push(pair<int,int>(1,maze[0].size() - 2));
            scatter_targets.push(pair<int,int>(1,maze[0].size() - 6));
            scatter_targets.push(pair<int,int>(3,maze[0].size() - 6));
            scatter_targets.push(pair<int,int>(3,maze[0].size() - 2));
        }
        else if(name == "Inky"){
            spawn_col = 12;
            spawn_row = 12;
            id = 2;
            scatter_targets.push(pair<int,int>(maze.size() - 4,8));
            scatter_targets.push(pair<int,int>(maze.size()-4,1));
            scatter_targets.push(pair<int,int>(maze.size()-2,1));
            scatter_targets.push(pair<int,int>(maze.size()-2, 10));
        }
        else if(name == "Clyde"){
            spawn_col = 11;
            spawn_row = 12;
            id = 3;
            scatter_targets.push(pair<int,int>(maze.size() - 4,maze[0].size() - 10));
            scatter_targets.push(pair<int,int>(maze.size()-4,maze[0].size() - 2));
            scatter_targets.push(pair<int,int>(maze.size()-2,maze[0].size() - 2));
            scatter_targets.push(pair<int,int>(maze.size()-2, maze[0].size() - 10));

        }
        x = 25 * spawn_col;
        y = 25 * spawn_row;
        speed = 1.8;
        sprite.setPosition(x + maze_offset_x, y + maze_offset_y);
    }

    void draw(sf :: RenderWindow& window){
        window.draw(sprite);
        flow_ghosts();
    }

  
  void toggle_sprite(){
        if(isEaten == true){
            sprite.setTexture(eaten_texture);
            sprite.setTextureRect(sf :: IntRect(25*4,0,25,25));
            scared_timer = 10;
            curr_frame = 0;
            speed = 2.5;
        }
        else if(isScared == true){
            sprite.setTexture(scared);
            sprite.setTextureRect(sf :: IntRect(0,0,25,25));
            scared_timer = 10;
            curr_frame = 0;
        }
        else{
            sprite.setTexture(text);
            sprite.setTextureRect(sf :: IntRect(25*4,0,25,25));
            scared_timer = 0;
            speed = 1.8;
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
            last_frame = 1;
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

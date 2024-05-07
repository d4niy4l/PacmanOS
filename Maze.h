#pragma once
#include "DungeonMaze.h"
#include "Animation.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
class Maze{
    public:
    vector<vector<int>>& maze;
    vector<vector<sf :: Sprite>> sprites;
    sf :: Texture tile_text;   
    sf :: Texture platform_text;
    sf :: Texture pellet_text;
    sf :: Font font;
    sf :: Text text;
    float timer;
    float speed;
    bool descended;
    Animation laser;
    Maze(string type,string tile_path, string platform_path) : descended(false), maze(dungeon),laser("./Sprites/Lazer.png",0.052,5,7){
        if(type == "dungeon"){
            maze = dungeon;
        }
    
        timer = 0;
        laser.setAnimationPosition(450,400);
        font.loadFromFile("./Fonts/Title_Font.ttf");
        tile_text.loadFromFile(tile_path);
        pellet_text.loadFromFile("./Sprites/Pellet.png");
        platform_text.loadFromFile(platform_path);
        text.setFont(font);
        text.setString("THE DUNGEON");
        text.setCharacterSize(35);
        text.setPosition(300,375);
        text.setFillColor(sf :: Color :: Black);
        speed = 0.90;
        for(int i = 0;i<maze.size();i++){
            sprites.push_back({});
            for(int j = 0;j<maze[i].size();j++){
                if(maze[i][j] == 0){
                    sprites[i].push_back(sf :: Sprite(tile_text));
                    sprites[i][j].setOrigin(tile_text.getSize().x/2, tile_text.getSize().y/2);
                    if(i < 11){
                        sprites[i][j].setPosition(j * 25 + maze_offset_x + 12.5, i* 25 + 12.5);
                    }
                    else{
                        sprites[i][j].setPosition(j * 25 + maze_offset_x + 12.5, i* 25 + 2*maze_offset_y + 12.5) ;

                    }
                    sprites[i][j].setScale(0,0);
                    sf :: Color color = sprites[i][j].getColor();
                    color.a = 0;
                    sprites[i][j].setColor(color);

                }
                else if(maze[i][j] == 4){
                    sprites[i].push_back(sf :: Sprite(platform_text));
                    sprites[i][j].setOrigin(tile_text.getSize().x/2, tile_text.getSize().y/2);
                    sprites[i][j].setPosition(j * 25 + maze_offset_x + 12.5, i* 25 + maze_offset_y + 12.5);
                    sprites[i][j].setScale(0,0);

                }
                else if(maze[i][j] == 2){
                    sprites[i].push_back(sf :: Sprite(pellet_text));
                    sprites[i][j].setOrigin(pellet_text.getSize().x/2, pellet_text.getSize().y/2);
                    sprites[i][j].setPosition(j * 25 + maze_offset_x + 12.5, i* 25 + maze_offset_y + 12.5);
                }
                else{
                    sprites[i].push_back(sf :: Sprite());
                }
            }
        }
    }
    void draw(sf :: RenderWindow& window, float time){
        
        bool descend = false;
        if(descended == false){
            for(int i = 0;i<maze.size();i++){
                for(int j = 0 ;j<maze[i].size();j++){
                    if(maze[i][j] == 0|| maze[i][j] == 4){
                        if(i < 11){
                            if(sprites[i][j].getPosition().y < i * 25 + maze_offset_y + 12.5){
                                sprites[i][j].move(0, speed);
                                window.draw(sprites[i][j]);
                                descend = true;
                            }
                            
                        }
                        else{
                            if(sprites[i][j].getPosition().y > i * 25 + maze_offset_y + 12.5){
                                sprites[i][j].move(0, -1 * speed);
                                window.draw(sprites[i][j]);
                                descend = true;
                            }
                        }
                        if(timer > 0.009 &&  sprites[i][j].getScale().x < 1){
                            sprites[i][j].setScale( sprites[i][j].getScale().x + 0.01,  sprites[i][j].getScale().y + 0.01);
                            sf :: Color color = sprites[i][j].getColor();
                            if(color.a < 255){
                                color.a += 2.5;
                                sprites[i][j].setColor(color);
                            }
                        }
                    }
                }
                if(descend == false){
                    descended = true;
                    timer = 0;
                }
            }
            laser.upDateAnimation();
            if(timer > 0.009){
                timer  = 0;
                sf :: Color color = laser.sprite.getColor();
                color.a -= 0.055;
                laser.sprite.setColor(color);
                color = sf :: Color(0,0,0,color.a);
                text.setFillColor(color);
            }
            window.draw(laser.sprite);
            window.draw(text);
        }
        else{
            for(int i = 0;i<dungeon.size();i++){
                for(int j = 0;j<dungeon[i].size();j++){
                    if(maze[i][j] != 1)
                        window.draw(sprites[i][j]);
                }
            }
        }
    }
    vector<int>& operator[](int index){
        if(index >= maze.size() || index < 0){
            std :: cout<<"Seg Fault\n";
            exit(EXIT_FAILURE);
        }
        return maze[index];
    }
    int size(){
        return maze.size();
    }
};
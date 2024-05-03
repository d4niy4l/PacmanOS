#pragma once
#include "DungeonMaze.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
class Maze{
    public:
    vector<vector<int>>& maze;
    vector<vector<sf :: Sprite>> sprites;
    sf :: Texture tile_texts;   
    bool descended;
    Maze(string type,string path) : descended(false), maze(dungeon){
        if(type == "dungeon"){
            maze = dungeon;
        }
        tile_texts.loadFromFile(path);
        for(int i = 0;i<maze.size();i++){
            sprites.push_back({});
            for(int j = 0;j<maze[i].size();j++){
                if(maze[i][j] == 0){
                    sprites[i].push_back(sf :: Sprite(tile_texts));
                    if(i < 11){
                        sprites[i][j].setPosition(j * 25 + 150, i* 25);
                    }
                    else{
                        sprites[i][j].setPosition(j * 25 + 150, i* 25 + 200);
                    }
                }
                else{
                    sprites[i].push_back(sf :: Sprite());
                }
            }
        }
    }
    void draw(sf :: RenderWindow& window){
        bool descend = false;
        if(descended == false){
                for(int i = 0;i<maze.size();i++){
                for(int j = 0;j<maze[i].size();j++){
                    if(maze[i][j] == 0){
                        if(i < 11){
                            if(sprites[i][j].getPosition().y < i * 25 + 100){
                                sprites[i][j].move(0, 0.1);
                                window.draw(sprites[i][j]);
                                descend = true;
                            }
                            
                        }
                        else{
                            if(sprites[i][j].getPosition().y > i * 25 + 100){
                                sprites[i][j].move(0, -0.1);
                                window.draw(sprites[i][j]);
                                descend = true;
                            }
                        }
                    }
                }
                if(descend == false){
                    descended = true;
                }
            }
        }
        else{
            for(int i = 0;i<dungeon.size();i++){
                for(int j = 0;j<dungeon[i].size();j++){\
                    if(dungeon[i][j] == 0){
                        window.draw(sprites[i][j]);
                    }
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
};
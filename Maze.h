#pragma once
#include "DungeonMaze.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
class Maze{
    public:
    vector<vector<int>>& maze;
    vector<vector<sf :: Sprite>> sprites;
    sf :: Texture tile_text;   
    sf :: Texture platform_text;
    bool descended;
    Maze(string type,string tile_path, string platform_path) : descended(false), maze(dungeon){
        if(type == "dungeon"){
            maze = dungeon;
        }
        tile_text.loadFromFile(tile_path);
        platform_text.loadFromFile(platform_path);
        for(int i = 0;i<maze.size();i++){
            sprites.push_back({});
            for(int j = 0;j<maze[i].size();j++){
                if(maze[i][j] == 0){
                    sprites[i].push_back(sf :: Sprite(tile_text));
                    if(i < 11){
                        sprites[i][j].setPosition(j * 25 + maze_offset_x, i* 25);
                    }
                    else{
                        sprites[i][j].setPosition(j * 25 + maze_offset_x, i* 25 + 2*maze_offset_y);
                    }
                }
                else if(maze[i][j] == 2){
                    sprites[i].push_back(sf :: Sprite(platform_text));
                    sprites[i][j].setPosition(j * 25 + maze_offset_x, i* 25);
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
                for(int j = 0 ;j<maze[i].size();j++){
                    if(maze[i][j] == 0|| maze[i][j] == 2){
                        if(i < 11){
                            if(sprites[i][j].getPosition().y < i * 25 + maze_offset_y){
                                sprites[i][j].move(0, 0.2);
                                window.draw(sprites[i][j]);
                                descend = true;
                            }
                            
                        }
                        else{
                            if(sprites[i][j].getPosition().y > i * 25 + maze_offset_y){
                                sprites[i][j].move(0, -0.2);
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
};
#pragma once
#include "Pacman.h"
#include "Ghost.h"
#include "Maze.h"
#include "pthread.h"
//  MUTEXES
pthread_mutex_t gameOverMutex;

//THREADS
array<pthread_t,4> ghostThreads;
pthread_t playerThread;
pthread_t gameThread;
pthread_t uiThread;

//  PLAYERS
Pacman pacman;
Maze maze("dungeon","./Sprites/Tile.png","./Sprites/Platform.png");
array<Ghost,4> ghosts;
sf::RenderWindow window;

//FONTS
sf :: Font hudFont;

//TEXTURES
sf :: Texture background_text;

//SPRITES
sf :: Sprite background;
//TEXTS
sf :: Text score;
sf :: Text score_int;
//GLOBALS
bool gameOver = false;
char pressed_dir;
float timer;
bool appeared = false;
float pactimer = 0;
array<float, 4> ghosttimers;
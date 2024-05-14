#pragma once
#include "Pacman.h"
#include "Ghost.h"
#include "Maze.h"
#include <pthread.h>
#include <semaphore.h>

enum{
    PELLETPRODUCEINTERVAL = 10
};

//  MUTEXES
pthread_mutex_t gameOverMutex;
pthread_mutex_t check_scared_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t checkCollision = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t checkPellet = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t checkSuper = PTHREAD_MUTEX_INITIALIZER;
// SEMAPHORES
sem_t space;
sem_t full;
sem_t mutex;
sem_t noEat;

//THREADS
array<pthread_t,4> ghostThreads;
pthread_t pacmanThread;
pthread_t gameThread;
pthread_t uiThread;
pthread_t consumerThread;
pthread_t producerThread;
//  PLAYERS
Pacman pacman;
array<Ghost,4> ghosts;
sf::RenderWindow window;

//FONTS
sf :: Font hudFont;

//TEXTURES
sf :: Texture background_text;
sf :: Texture noLife;
sf :: Texture Life;
//SPRITES
sf :: Sprite background;
array<sf :: Sprite, 3> lives;
//TEXTS
sf :: Text score;
sf :: Text score_int;

//GLOBALS
bool gameOver = false;
char pressed_dir;
float timer;

bool appeared = false;
float pactimer = 0;
float clyde_timer = 0;
bool clyde_psudoscatter = false;
array<float, 4> ghosttimers;
bool pacAte = false; //boolean for lightning
Animation lightening = Animation("./Sprites/Lightning.png", 0.05,4,2);
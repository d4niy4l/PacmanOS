#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;

class Button{
    public:
    sf :: Font font;
    sf :: Text text;
    sf :: RectangleShape panel;
    bool isActive;
    float timer;
    Button() : panel(sf :: Vector2f(200,200)){
        panel.setPosition(400,400);
        panel.setFillColor(sf :: Color :: Black);
        panel.setOutlineColor(sf :: Color :: White);
        panel.setOutlineThickness(1);
        font.loadFromFile("./Fonts/Button_Font.ttf");
        text.setFont(font);
        text.setFillColor(sf :: Color :: White);
        text.setString("Start");
        text.setPosition(250,250);
        text.setCharacterSize(30);
    };
};

class Menu{
    public:
    vector<Button> options;
    Menu(){

    }

};



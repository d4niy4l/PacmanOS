#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
class Animation
{
	int currentImageX;
	int totalX;
	int currentImageY;
	int totalY;
	float width;
	float height;
	float topLeft;
	float top;
	sf::Texture tex;
public:
	float timer;
	float delay;
	sf::Sprite sprite;
    Animation(std::string filePath, float timeDelay, int countX, int countY) {
        tex.loadFromFile(filePath);
        totalX = countX;
        totalY = countY;
        width = tex.getSize().x / countX;
        height = tex.getSize().y / countY;
        topLeft = 0.0;
        top = 0;
        delay = timeDelay;
        timer = 0;
        currentImageX = 0;
        currentImageY = 0;
        sprite.setTexture(tex);
        sprite.setOrigin(width * 0.5, height * 0.5);
        sprite.setTextureRect(sf :: IntRect(0, 0, width, height));
    }
    void setAnimationPosition(float x, float y) {
	    sprite.setPosition(x, y);
    }

	void upDateAnimation() {
        if (timer >= delay) {
            timer = 0;
            currentImageX++;
            if (currentImageY < totalY && currentImageX >= totalX) {
                currentImageY++;
                currentImageX = 0;
            }
        }
        topLeft = currentImageX * width;
        top = currentImageY * height;
        sprite.setTextureRect(sf::IntRect(topLeft, top, width, height));
    }   

	void restartAnimation() {
			topLeft = 0;
			top = 0;
			currentImageX = 0;
			currentImageY = 0;
			timer = 0;
		
	}
	bool animationFinished() {
	if (currentImageX >= totalX && currentImageY >= totalY)
		return true;
	return false;
}
};
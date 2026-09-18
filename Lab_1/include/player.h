#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
	Player();
	void Draw(sf::RenderWindow& window);
	void Update();

private:
	sf::RectangleShape playerRect;
};